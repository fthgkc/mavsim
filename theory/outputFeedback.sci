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
    //disp("norm of ricatti for P"); disp(norm(Ac'*P+P*Ac+Bc'*R*Bc+Q))
    S = riccati(Ac',zeros(nX,nX),X,'c'); // dH/dP
    //disp("norm of ricatti for S"); disp(norm(Ac*S+S*Ac'+X));
    f = 1/2*trace(P*X); // cost
    //disp(f)
    dH_dK = 2*(R*K*C*S*C' - B'*P*S*C'); // dH/dK
    g = matrix(dH_dK,size(dH_dK,1)*size(dH_dK,2),1);
endfunction

function K = outputFeedbackDesign(K0,sys,Q,R,sigX0,K0)
    [A,B,C,D] = abcd(sys);
    nX = size(A,1);
    nU = size(B,2);
    nY = size(C,1);

    [f,xopt] = optim(list(outputFeedbackCost,sys,Q,R,sigX0),K0);
    if (f >= 0)
        printf("\noutput feedback cost: %f\n",f);
    elseif (f < 0)
        printf("\noutput feedback failed to converge, try another initial guess\n");
    end
    K = matrix(xopt,nU,nY)
endfunction

nX = 4;
nY = 2;
nU = 2;
sys = tf2ss([20/(1+%s),20/(2+%s);20/(3+%s),20/(4+%s)]);
Q = eye(nX,nX);
R = eye(nU,nU);
sigX0 = ones(nX,1);
K0 = 0.01*ones(nY,nU);

outputFeedbackDesign(K0,sys,Q,R,sigX0,K0)

// vim:expandtab:ts=4:sw=4
