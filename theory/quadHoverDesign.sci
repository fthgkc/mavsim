mode(-1);

function data = quadHoverDesign(H)

    theta = 0;
	data.trim.theta = theta;
    T_sumSq_trim= 0.5;
    data.trim.T_sumSq = T_sumSq_trim;
    T_sumSq=T_sumSq_trim;
    dutycycle_L=T_sumSq/4; dutycycle_R=T_sumSq/4;
    dutycycle_F=T_sumSq/4; dutycycle_B=T_sumSq/4;

    // include dynamics
    exec quad_body_dynamics.sci;
    olss = syslin('c',F_body_quad,G_body_quad,C_body_quad,D_body_quad);
    oltf = clean(ss2tf(olss),1e-8);

    // define variables
    x = createIndex(["U" "W" "wy" "h" "V" "phi" "wx" "psi" "wz" "dcL" "dcR" "dcF" "dcB"]);
    y = createIndex(["U" "W" "theta" "wy" "h" "V" "phi" "wx" "psi" "wz"]);
    u = createIndex(["Sum" "FB" "LR" "LR_FB"]);

    // save to data structure
    data.x = x; data.y = y; data.u = u;
    data.olss = olss; data.oltf = oltf;

    // initialize closed loop as open before closures
    data.clss = olss; data.cltf = oltf;


    printf("\n\nQuadrotor Hover : Classical Design\n");
    printf("=================================================================================\n");

    // controllers
    data = closeLoop(data,      data.y.wx,      data.u.LR,      H(1,1));
    data = closeLoop(data,      data.y.wy,      data.u.FB,      H(2,1));
    data = closeLoop(data,      data.y.phi,     data.u.wx,      H(3,1));
    data = closeLoop(data,      data.y.theta,   data.u.wy,      H(4,1));
    data = closeLoop(data,      data.y.U,       data.u.theta,   H(5,1));
    data = closeLoop(data,      data.y.V,       data.u.phi,     H(6,1));
    data = closeLoop(data,      data.y.W,       data.u.Sum,     H(7,1)); 
    data = closeLoop(data,      data.y.psi,     data.u.LR_FB,   H(8,1)); 
    data = closeLoop(data,      data.y.h,       data.u.W,       H(9,1));
    // The order that you close the loops here matters. The inner loop will have a higher bandwidth.

    // the error catching is used in case the user had commented a loop above
    // isdef doesn't work for structure variables yet
    execstr("loopAnalysis(data.cltf,data.y.wx,data.u.wx,data.u.str(data.u.wx))","errcatch");
    execstr("loopAnalysis(data.cltf,data.y.phi,data.u.phi,data.u.str(data.u.phi))","errcatch");
    execstr("loopAnalysis(data.cltf,data.y.wy,data.u.wy,data.u.str(data.u.wy))","errcatch");
    execstr("loopAnalysis(data.cltf,data.y.wz,data.u.wz,data.u.str(data.u.wz))","errcatch");
    execstr("loopAnalysis(data.cltf,data.y.theta,data.u.theta,data.u.str(data.u.theta))","errcatch");
    execstr("loopAnalysis(data.cltf,data.y.psi,data.u.psi,data.u.str(data.u.psi))","errcatch");
    execstr("loopAnalysis(data.cltf,data.y.Vt,data.u.Vt,data.u.str(data.u.Vt))","errcatch");
    execstr("loopAnalysis(data.cltf,data.y.h,data.u.h,data.u.str(data.u.h))","errcatch");

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

    printf("\n\nQuadrotor Hover : Modern LQG Design Analysis\n");
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
    K.tf = clean(ss2tf(K.ss));
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
