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

function [J,dJ_dK,ind] = costLqt(x,ind,varargin)
	// pg. 428 Lewis and Stevens

	// system info
	sys=varargin(1);

	// performance index 
	P=varargin(2); Q=varargin(3); R=varargin(4);

	// time weighting, power of to weight by
	timeK=varargin(5);

	// compensator
	G=varargin(6); F=varargin(7);
	KFixed=varargin(8);

	// sys info
	[A,B,C,D] = abcd(sys);
    nX = size(A,1); nU = size(B,2); nY = size(C,1);

	// optimize for step input on all channels
	r0=ones(size(G,2),1);

	// gain
    K = matrix(x,nU,nY);

	// controlled matrices
    Ac = A-B*K*C;
    Bc = G-B*K*F;

	xBar = -(Ac)^-1*Bc*r0;
	yBar = C*xBar+F*r0;
    X = xBar*xBar';

	// solve for dH/dS = 0 for Pk
	Pk(1,:,:) = P;
	for k=1:(timeK-1)
		Pk(k+1,:,:) = riccati(Ac,zeros(nX,nX),matrix(Pk(k,:,:),nX,nX),'c');
	end
	if(timeK < 1)
		Qc = Q+C'*K'*R*K*C;
	else
		Qc = factorial(timeK)*matrix(Pk(timeK,:,:),nX,nX)+Q+C'*K'*R*K*C;
	end 
	Pk(timeK+1,:,:) = riccati(Ac,zeros(nX,nX),Qc,'c'); // dH/dS
	Pk1 = matrix(Pk(timeK+1,:,:),nX,nX);
	normP = norm(Ac'*Pk1 + Pk1*Ac+Qc);

	// solve for dH/dP = 0 for Sk
	Sk(timeK+1,:,:) = X;
	if (timeK>0)
		Sk(timeK,:,:) = riccati(Ac',zeros(nX,nX),..
			factorial(timeK)*matrix(Sk(timeK+1,:,:),nX,nX),'c'); // dH/dS
	end
	if (timeK>2)
		for k=(timeK-2):0
			disp(k)
			Sk(k+1,:,:) = riccati(Ac',zeros(nX,nX),matrix(Sk(k,:,:),nX,nX),'c'); // dH/dS
		end
	end
	PkSkSum = matrix(Pk(1,:,:),nX,nX) * matrix(Sk(1,:,:),nX,nX);
	for k=1:timeK+1
		PkSkSum = PkSkSum + matrix(Pk(k,:,:),nX,nX)*matrix(Sk(k,:,:),nX,nX);
	end

	// compute cost
    J = 1/2*trace(Pk1*X); // cost

	// cleanup

	// calculate dH/dK = dJ/dK (since H=J when lyap hold, g=0) for gradient based methods
	Sk1 = matrix(Sk(timeK+1,:,:),nX,nX);
	dJ_dK = R*K*C*Sk1*C'-B'*PkSkSum*C'+B'*(Ac')^-1*Pk1*xBar*yBar';

	// check solution
	if (normP>1e-3)
		printf("\nWARNING: riccati solution failed\n");
		printf("\tnorm of ricatti for P: %f\n",normP);
		ind=-1; // riccati solution failed
		J = 1e9;
		info=-1;
	end
	//disp("K:");disp(K)
	printf("J: %f\tgradient norm: %f\n",J,norm(dJ_dK));
	//disp("K convergence"); disp(K - R^-1*B'*P*S*C'*(C*S*C')^-1);
	//printf("\tnorm of ricatti for P: %f\n",normP);
	//halt()
endfunction

function [K,gradopt,info] = lqtDesign(sys,P,Q,R,timeK,G,F,KFixed,K0)
    [A,B,C,D] = abcd(sys);
	gradopt = []; K = [];
	size(Q)
	size(A)
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
	ind=0;
    [fopt,xopt,gradopt] = optim(list(NDcost,costLqt,ind,sys,P,Q,R,timeK,G,F,KFixed),x0,..
		"qn","ar",1e3,1e3,1e-4,1e-4,imp=1);
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
nX = size(A,1); nU = size(B,2); nY = size(C,1);
nR = 2;
sys = syslin('c',A,B,C);
P = 0.001*eye(nX,nX);
Q = diag([50,100,100,50,0,0,1]);
R = 0.0001*eye(nU,nU);
G = zeros(nX,nR);
F = eye(nY,nR);
KFixed = [];
timeK = 2;
// answer you should get from the book, pg. 416
KBook = [-.56,-.44,0.11,-0.35;-1.19,-.21,-0.44,0.26];
K0 = KBook;
sigX0 = 1*ones(7,1);

[K,gradopt,info] = lqtDesign(sys,P,Q,R,timeK,G,F,KFixed,K0);
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

