lines(0);
clc; clear;

// Solves output feedback problem for optimal
// gains given cost function 
// 
// J = 1/2 * integral ( x'.Q.x + u'.R.u, dt)
//
// subject to the constraint:
// 
//      dx/dt = A.x + B.u
//      u = -Ky
//
// based on Aircraft Control and Simulation
//          Lewis and Stevs, 2003
//
//          see pg. 405, 2nd edition
//
// Q : state error waiting
// R : input error waiting
// sigX0 : initial standard deviaton in x0
// K0 : initial guess of stabilizing gain
//
// outputFeedbackDesign given the above arguments
// will calculate K such that J is minimized over
// the inifinite time horizon

function [f,g,ind] = outputFeedbackCost(x,ind,sys,Q,R,sigX0)
    [A,B,C,D] = abcd(sys);
    nX = size(A,1);
    nU = size(B,2);
    nY = size(C,1);
    K = matrix(x,nU,nY);
    Ac = A-B*K*C;
    Bc = K*C;
    X = sigX0*sigX0';
    P = riccati(Ac,zeros(nX,nX),Bc'*R*Bc+Q,'c'); // dH/dS
    normP = norm(Ac'*P+P*Ac+Bc'*R*Bc+Q);
    //disp("norm of ricatti for P"); disp(normP)
    S = riccati(Ac',zeros(nX,nX),X,'c'); // dH/dP
    normS = norm(norm(Ac*S+S*Ac'+X));
    //disp("norm of ricatti for S"); disp(normS);
    f = 1/2*trace(P*X); // cost
    disp(f)
    dJ_dK = R*K*C*S*C' - B'*P*S*C'; // dJ/dK
    g = matrix(dJ_dK,nU*nY,1);
    //disp("g"); disp(g)
    //disp("closed loop eigen values"); disp(spec(A-B*K*C))

    if (normP>1 | normS > 1)
        ind=-1; // riccati solution failed
    end
endfunction

function [K,info] = outputFeedbackDesign(sys,Q,R,sigX0,K0,cFixed)
    [A,B,C,D] = abcd(sys);
    nX = size(A,1);
    nU = size(B,2);
    nY = size(C,1);
    [fopt,xopt,gradopt] = optim(list(outputFeedbackCost,sys,Q,R,sigX0),K0);
    K = matrix(xopt,nU,nY)
    if (fopt >= 0)
        printf("\noutput feedback cost: %f, gradient: %f\n",fopt,gradopt);
        printf("\neigen values: \n");
        disp(spec(A-B*K*C))
        info = 0;
    elseif (max(real(spec(A-B*K))>0))
        printf("\nclosed loop not stable\n");
        info = 1;
    elseif (fopt < 0)
        printf("\noutput feedback failed to converge, try another initial guess\n");
        info = 1;
    end
endfunction

// F-16 lateral regulator
// example from pg. 414

A = [-0.3220,0.0640,0.0364,-0.9917,0.0003,0.0008,0;
     0,0,1,0.0037,0,0,0;
    -30.6492,0,-3.6784,0.6646,-0.7333,0.1315,0;
     8.5396,0,-0.0254,-0.4764,-0.0319,-0.0620,0;
    0,0,0,0,-20.2,0,0;
    0,0,0,0,0,-20.2,0;
    0,0,0,57.2958,0,0,-1];
B = [0,0;0,0;0,0;0,0;20.2,0;0,20.2;0,0];
C = [0,0,0,57.2958,0,0,-1;
     0,0,57.2958,0,0,0,0;
     57.2958,0,0,0,0,0,0;
     0,57.2958,0,0,0,0,0];

sys = syslin('c',A,B,C);
qDutchRoll = 100;
qRoll = 50;
rho = 0.1;
Q = diag([qDutchRoll,qRoll,qRoll,qDutchRoll,0,0,1]);
R = rho^2*eye(2,2);
// answer you should get from the book, pg. 416
KBook = [-.56,-.44,0.11,-0.35;-1.19,-.21,-0.44,0.26];
disp("initial stabilizing gains");
sigX0 = 0*ones(7,1);

[K,info] = outputFeedbackDesign(sys,Q,R,sigX0,KBook,[])

sysC = syslin('c',A-B*K*C,B,C);
sysCBook = syslin('c',A-B*KBook*C,B,C);
t = linspace(0,10,1000); u = zeros(1,size(t,2));
x0 = zeros(7,1); x0(1) = 1.0*%pi/180;
scf(1); clf(); xgrid;
title("design");
plot2d(t,csim(u,t,sysC(:,1),x0)',leg="r@p@beta@phi");
scf(2); clf(); xgrid;
title("design from book");
plot2d(t,csim(u,t,sysCBook(:,1),x0)',leg="r@p@beta@phi");
