lines(0);
clc; clear;
mode(-1);

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

function f = costLqr(x,varargin)
	sys=varargin(1);
	Q=varargin(2);
	R=varargin(3);
	sigX0=varargin(4);
    [A,B,C,D] = abcd(sys);
    nX = size(A,1);
    nU = size(B,2);
    nY = size(C,1);
    K = matrix(x,nU,nY);
    Ac = A-B*K*C;
    X = sigX0*sigX0';
	P = riccati(Ac,zeros(nX,nX),C'*K'*R*K*C+Q,'c'); // dH/dS
    normP = norm(Ac'*P+P*Ac+C'*K'*R*K*C+Q);
    f = 1/2*trace(P*X); // cost
    if (normP>1e-3)
		printf("\nWARNING: riccati solution failed\n");
		printf("\tnorm of ricatti for P: %f\n",normP);
        ind=-1; // riccati solution failed
		f = 1e9;
    end
	//disp("K:");disp(K)
	//disp("f:");disp(f);
	//disp("K convergence"); disp(K - R^-1*B'*P*S*C'*(C*S*C')^-1);
	//printf("\tnorm of ricatti for P: %f\n",normP);
	//halt()
endfunction

function [f,g,ind] = costLqr2(x,ind,sys,Q,R,sigX0)
	// provides gradient
    [A,B,C,D] = abcd(sys);
    nX = size(A,1);
    nU = size(B,2);
    nY = size(C,1);
    K = matrix(x,nU,nY);
	disp("K:");disp(K)
    Ac = A-B*K*C;
    X = sigX0*sigX0';
    P = riccati(Ac,zeros(nX,nX),C'*K'*R*K*C+Q,'c'); // dH/dS
    normP = norm(Ac'*P+P*Ac+C'*K'*R*K*C+Q);
    S = riccati(Ac',zeros(nX,nX),X,'c'); // dH/dP
    normS = norm(norm(Ac*S+S*Ac'+X));
    f = 1/2*trace(P*X); // cost
    dJ_dK = R*K*C*S*C' - B'*P*S*C'; // dJ/dK
    if (normP>1e-3 | normS > 1e-3)
		//printf("\nWARNING: riccati solution failed\n");
        //printf("\tnorm of ricatti for P: %f\n",normP);
        //printf("\tnorm of ricatti for S: %f\n",normS);
        ind=-1; // riccati solution failed
		dJ_dK = rand(size(dJ_dK,1),size(dJ_dK,2));
		f = 1e9;
    end
    g = matrix(dJ_dK,nU*nY,1);
	disp("g:");disp(g);
	disp("K convergence"); disp(K - R^-1*B'*P*S*C'*(C*S*C')^-1);
	halt()
endfunction

function J = costLqt(x,varargin)
	mode(1)
	// pg. 428 Lewis and Stevens
	sys=varargin(1);

	// compensator
	G=varargin(2); F=varargin(3);
	
	// performance index 
	P=varargin(4); Q=varargin(5); R=varargin(6);
	
	// time weighting, power of to weight by
	timeK=varargin(7);

	// optimize for step input on all channels
	r0=ones(size(G,2),1);

	// system info
    [A,B,C,D] = abcd(sys);
    nX = size(A,1); nU = size(B,2); nY = size(C,1);

	// gain
    K = matrix(x,nU,nY);

	// controlled matrices
    Ac = A-B*K*C;
    Bc = G-B*K*F;

	xBar = -(Ac)^-1*Bc*r0;
	yBar = C*xBar+F*r0;
    X = xBar*xBar';

	// solve for dH/dS = 0 for Pk
	disp("solving for Pk");
	Pk(1,:,:) = P;
	for k=1:(timeK-1)
		Pk(k+1,:,:) = riccati(Ac,zeros(nX,nX),Pk(k,:,:),'c');
	end
	Qc = factorial(timeK)*Pk(timeK,:,:)+Q+C'*K'*R*K*C;
	Pk(timeK+1,:,:) = riccati(Ac,zeros(nX,nX),Qc,'c'); // dH/dS
    normP = norm(Ac'*Pk(timeK+1,:,:)+P(timeK+1,:,:)*Ac+Qc);

	// solve for dH/dP = 0 for Sk
	disp("solving for Sk");
	Sk(timeK+1,:,:) = X;
	if (timeK>0)
		Sk(timeK,:,:) = riccati(Ac',zeros(nX,nX),..
			factorial(timeK)*Sk(timeK+1,:,:),'c'); // dH/dS
	end
	for i=(timeK-2):0
		Sk(k+1,:,:) = riccati(Ac',zeros(nX,nX),Sk(k,:,:),'c'); // dH/dS
	end
	PkSkSum = Pk(1,:,:) + Sk(1,:,:);
	for k=1:timeK+1
		PkSkSum = PkSkSum + Pk(k,:,:)*Sk(k,:,:);
	end

	// calculate dH/dK = dJ/dK (since H=J when lyap hold, g=0) for gradient based methods
	disp("solving for dJ/dK");
	dJ_dK = R*K*C*Sk(timeK+1,:,:)*C'-B'*PkSkSum*C'+B'*(Ac')^-1*Pk(timeK+1,:,:)*xBar*yBar';

	// compute cost
	disp("solving for J");
    J = 1/2*trace(P*X) + 1/2*eBar'*V*eBar; // cost

	// check solution
    if (normP>1e-3)
		printf("\nWARNING: riccati solution failed\n");
		printf("\tnorm of ricatti for P: %f\n",normP);
        ind=-1; // riccati solution failed
		J = 1e9;
    end
	//disp("K:");disp(K)
	//disp("f:");disp(f);
	//disp("K convergence"); disp(K - R^-1*B'*P*S*C'*(C*S*C')^-1);
	//printf("\tnorm of ricatti for P: %f\n",normP);
	//halt()
endfunction

function [K,gradopt,info] = lqtDesign(sys,Q,R,sigX0,K0,cFixed)
	mode(1)
    [A,B,C,D] = abcd(sys);
	gradopt = []; K = [];
	if(rank(obsv_mat(A,sqrt(Q))) < rank(A))
		error("Problem not observable, weight more elements of Q");	
		info = 1;
		return;
	elseif(rank(R) < size(B,2))
		error("You must weight all elements of R");	
		info = 2;
		return;
	elseif(max(real(spec(A-B*K0*C))) >0)
		//disp("Unstable initial gain.");	
		info = 3;
		return;
	end

    nX = size(A,1);
    nU = size(B,2);
    nY = size(C,1);
	x0 = matrix(K0,nY*nU,1);
    [fopt,xopt,gradopt] = optim(list(NDcost,costLqr,sys,Q,R,sigX0),x0,..
		"qn","ar",1e3,1e3,imp=1);
    K = matrix(xopt,nU,nY)
    if (fopt >= 0)
        //printf("\noutput feedback cost: %f, gradient: %f\n",fopt,gradopt);
        //printf("\neigen values: \n"); disp(spec(A-B*K*C))
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
rho = 0.1;
Q = diag([50,100,100,50,0,0,1]);
R = rho*eye(2,2);
// answer you should get from the book, pg. 416
KBook = [-.56,-.44,0.11,-0.35;-1.19,-.21,-0.44,0.26];
K0 = KBook;
sigX0 = 1*ones(7,1);

//while(1)
	//K0 = 1*(rand(2,4)-0.5);
	mode(1);
	[K,gradopt,info] = lqtDesign(sys,Q,R,sigX0,K0,[]);
//end
sysC = syslin('c',A-B*K*C,B,C);
sysCBook = syslin('c',A-B*KBook*C,B,C);
t = linspace(0,10,1000); u = zeros(1,size(t,2));
x0 = zeros(7,1); x0(1) = 1.0*%pi/180;


disp("eigen values of open loop"); disp(spec(A))
disp("eigen values of closed loop design from book"); disp(spec(sysCBook.A))
disp("eigen values of closed loop design new"); disp(spec(sysC.A))
disp(K,"K")


scf(1); clf();
subplot(3,2,1);
title("design");
[sim.y,sim.x] = csim(u,t,sysC(:,1),x0);
plot2d(t,sim.y',leg="r@p@beta@phi");
subplot(3,2,2);
title("design input");
plot2d(t,(K*sim.y)',leg="aileron@elevator");

subplot(3,2,3);
title("design from book");
[sim.y,sim.x] = csim(u,t,sysCBook(:,1),x0);
plot2d(t,sim.y',leg="r@p@beta@phi");
subplot(3,2,4);
title("design book input");
plot2d(t,(KBook*sim.y)',leg="aileron@elevator");

subplot(3,2,5);
title("open loop");
[sim.y,sim.x] = csim(u,t,sys(:,1),x0);
plot2d(t,sim.y',leg="r@p@beta@phi");

