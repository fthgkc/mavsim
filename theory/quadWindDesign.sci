mode(-1);

exec unityFeedback.sci;

function  infoNew = createIndex(names,info)
    // Used to define string/ indices for system

    // check if we are appending
    if (argn(2) == 2)
        iStart = max(size(info.str));
    else
        iStart = 0;
    end

    // add strings
     for i=1:max(size(names))
        execstr('info.str(i+iStart)=names(i)')
    end

    // add new indices
    for i=1:max(size(names))
        execstr('info.'+names(i)+'=i+iStart')
    end
    
    // output modified info
    infoNew = info;
endfunction

function dataNew = closeLoop(data,y,u,H);
    printf("\tclosing %8s -> %8s loop\n", data.u.str(u), data.y.str(y));
    data.clss = unityFeedback(y,u,data.clss,H);
    data.cltf = clean(ss2tf(data.clss),1e-8);
    data.u = createIndex(data.y.str(y),data.u);
    dataNew = data;
endfunction

function loopAnalysis(data,y,u)
    sys = data.cltf(y,u);
    pm = p_margin(sys);
    gm = g_margin(sys);
    if ( size(pm) == 0) pm = %inf; end
    if ( size(gm) == 0) gm = %inf; end
    if ( min(gm) < 0 | min(pm) > 0)
        stability = "unstable";
    else 
        stability = "stable";
    end
    printf("%8s -> %8s\tgain margin: %6.1f\tphase margin: %6.1f\t%s\n",..
        data.u.str(u),data.y.str(y),min(gm),min(pm),stability);
endfunction

function data = quadWindDesign()

    // solve for pitch angle at trim;
    deff('[y]=theta_sol(x)','y=(-4*%pi^2*rho*x^2*sin(x)*K_cd_cl-rho*sin(x)*'+..
           'Cd0-2*%pi*rho*x*cos(x))*s_frame*Vt^2+(2*cos(x)*sin(x)^2+2*cos(x)^3)*g*m');
    theta=fsolve(-17*%pi/180,theta_sol)
    alpha=theta;
    qwd.trim.theta=theta;

    // solve for T_sumSq at trim
    //check the equation below; there was a minus sign in front
    T_sumSq_trim= ((1800*%pi^2*rho*theta^2*K_cd_cl+450*rho*Cd0)*..
    s_frame*Vt^2)/(%pi^3*cos(theta)*rho*batVolt^2*KV^2*rBlade^4*C_T)
    qwd.trim.T_sumSq = T_sumSq_trim;
    T_sumSq=T_sumSq_trim;
    dutycycle_L=T_sumSq/4; dutycycle_R=T_sumSq/4;
    dutycycle_F=T_sumSq/4; dutycycle_B=T_sumSq/4;

    // include dynamics
    exec quad_wind_dynamics.sci;
    olss = syslin('c',F_wind_quad,G_wind_quad,C_wind_quad,D_wind_quad);
    oltf = clean(ss2tf(olss),1e-8);

    // define variables
    x = createIndex(["Vt" "alpha" "theta" "wy" "h" "beta" "phi" "wx" "psi" "wz" "dcL" "dcR" "dcF" "dcB"]);
    y = createIndex(["Vt" "theta" "wy" "h" "phi" "wx" "psi" "wz"]);
    u = createIndex(["Sum" "FB" "LR" "LR_FB"]);

    // save to data structure
    data.x = x; data.y = y; data.u = u;
    data.olss = olss; data.oltf = oltf;

    // initialize closed loop as open before closures
    data.clss = olss; data.cltf = oltf;


    clc;
    printf("\n\nClassical Design\n");
    printf("=================================================================================\n");

    // controllers
    data = closeLoop(data,      data.y.wx,      data.u.LR,      1 + 0/%s + .05*%s/(%s+20));
    data = closeLoop(data,      data.y.wy,      data.u.FB,      1 + 0/%s + .05*%s/(%s+20));
    data = closeLoop(data,      data.y.phi,     data.u.wx,      0.5 + 1.0/%s + 0*%s/(%s+20));
    data = closeLoop(data,      data.y.theta,   data.u.wy,      0.1 + 1.0/%s + 0*%s/(%s+20));
    data = closeLoop(data,      data.y.Vt,      data.u.Sum,     .1 + 0/%s + .1*%s/(%s+20));
    data = closeLoop(data,      data.y.psi,     data.u.phi,     0.0 + 1.0/%s + 0*%s/(%s+20));
    data = closeLoop(data,      data.y.h,       data.u.theta,   0.01 + 0.2/%s + 0*%s/(%s+20));
    // The order that you close the loops here matters. The inner loop will have a higher bandwidth.

    // the error catching is used in case the user had commented a loop above
    // isdef doesn't work for structure variables yet
    execstr("loopAnalysis(data,data.y.wx,data.u.wx)","errcatch");
    execstr("loopAnalysis(data,data.y.phi,data.u.phi)","errcatch");
    execstr("loopAnalysis(data,data.y.wy,data.u.wy)","errcatch");
    execstr("loopAnalysis(data,data.y.wz,data.u.wz)","errcatch");
    execstr("loopAnalysis(data,data.y.theta,data.u.theta)","errcatch");
    execstr("loopAnalysis(data,data.y.psi,data.u.psi)","errcatch");
    execstr("loopAnalysis(data,data.y.Vt,data.u.Vt)","errcatch");
    execstr("loopAnalysis(data,data.y.h,data.u.h)","errcatch");

    [eVec, eVal] = spec(data.clss.A);
    data.eVec = eVec;
    data.eVal = eVal;
    if (max(real(diag(eVal))) > 0)
        printf("\n\tWARNING: UNSTABLE!!!!!");
        [maxVal, maxI] = max(real(diag(eVal)));
        printf("\n\tfastest unstable time constant: %f Hz",maxVal/(2*%pi));
    else
        printf("\n\tSTABLE");
        [minVal, minI] = max(real(diag(eVal)));
        printf("\n\tslowest stable time constant: %f Hz",abs(minVal/(2*%pi)));
    end

    printf("\n\nModern LQG Design Analysis\n");
    printf("=================================================================================\n");
    // modern control design
    nX = size(olss.A,1);
    nY = size(olss.C,1);
    nU = size(olss.B,2);

    // set the Q and R matrices to weight importance in your controller
    Q = eye(nX+nU,nX+nU); // input trim deviation penality
    R = eye(nX+nY,nX+nY); // measurement error  penality
    [P,r] = lqg2stan(minss(olss),Q,R);
    K.ss = minss(lqg(P,r));
    K.tf = clean(ss2tf(K.ss))
    [eVec, eVal] = spec(h_cl(P,r,K.ss));
     if (max(real(diag(eVal))) > 0)
        printf("\n\tWARNING: UNSTABLE!!!!!");
        [maxVal, maxI] = max(real(diag(eVal)));
        printf("\n\tfastest unstable time constant: %f Hz",maxVal/(2*%pi));
    else
        printf("\n\tSTABLE");
        [minVal, minI] = max(real(diag(eVal)));
        printf("\n\tslowest stable time constant: %f Hz",abs(minVal/(2*%pi)));
    end
    printf("\n\tLQG state space sizes\ty:%d\tu:%d\tx:%d",size(K.ss,1),size(K.ss,2),size(K.ss.A,1));
    data.K_modern.ss = K.ss;
    data.K_modern.tf = K.tf;

endfunction

// vim:ts=4:sw=4:expandtab
