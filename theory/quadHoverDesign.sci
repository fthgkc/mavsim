mode(-1);

function data = quadHoverDesign(H)
    theta = 0;
	data.trim.theta = theta;
	T_sumSq_trim = 900*g*m/(%pi^3*rho*batVolt^2*KV^2*rBlade^4*C_T);
	printf("\n\tT_sumSq_trim: %f\n", T_sumSq_trim);
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
    data.x = x; data.y = y; data.u = u; data.H = H;
    data.olss = olss; data.oltf = oltf;

    // initialize closed loop as open before closures
    data.clss = olss; data.cltf = oltf;


    printf("\n\nQuadrotor Hover : Classical Design\n");
    printf("=================================================================================\n");
    printf("\ty\t\tu\tstability\tgain\tphase\tband\n");
    printf("\t\t\t\t\t\tmargin\tmargin\twidth\n");
    printf("---------------------------------------------------------------------------------\n");
    // controllers
    data = closeLoop(data,      data.y.wx,      data.u.LR,      H.wx_LR);
    data = closeLoop(data,      data.y.wy,      data.u.FB,      H.wy_FB);
    data = closeLoop(data,      data.y.wz,      data.u.LR_FB,   H.wz_LR_FB);
    data = closeLoop(data,      data.y.W,       data.u.Sum,     H.W_Sum);
    data = closeLoop(data,      data.y.phi,     data.u.wx,      H.phi_wx);
    data = closeLoop(data,      data.y.theta,   data.u.wy,      H.theta_wy);
    data = closeLoop(data,      data.y.psi,     data.u.wz,      H.psi_wz); 
    data = closeLoop(data,      data.y.U,       data.u.theta,   H.U_theta);
    data = closeLoop(data,      data.y.V,       data.u.phi,     H.V_phi);
    data = closeLoop(data,      data.y.h,       data.u.W,       H.h_W);
    // The order that you close the loops here matters. The inner loop will have a higher bandwidth.

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

    printf("\n\nQuadrotor Hover : Modern LQG Regulator Design Analysis\n");
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
