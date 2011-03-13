mode(-1)
exec steadycos.sci // load custom steadycos using NDcost
load pixhawk.cos
x = createIndex(["U","W","theta","wy","V","phi","wx","psi","wz","LR","FB","LRFB","SUM","pN","pE","h"]);
y = x;
u = createIndex(["LR","FB","LRFB","SUM"]);

dynamics=scs_m.objs(1).model.rpar;
controller=scs_m.objs(3).model.rpar;
motorLag=scs_m.objs(96).model.rpar;


// dynamics block

// vary u to find zero initial conitions
[X,U,Y,XP] = steadycos(dynamics,[],[],[],[],[1:$],[],[]);
X=clean(X,1e-5);
U=clean(U,1e-5);
quadTf = clean(ss2tf(lincos(dynamics,X,U)),1e-5);

// motor lag block

// poles are at input from dynamics
UM=U;
motorLagTf = clean(ss2tf(lincos(motorLag,zeros(4,1),UM)),1e-5);

sys.oltf = quadTf*motorLagTf
sys.olss = minssAutoTol(tf2ss(sys.oltf),16);

function openLoopAnalysis(name,sys)
	//sse
	sse=1/(horner(sys,1e-10));
	if (sse>1e6) sse=%inf; end

	// pm/gm
	if (sys == 0)
        pm = -%inf;
        gm = -%inf;
    else
		pm = p_margin(sys)+180;
        for i=1:length(pm)
            if (pm(i) >= 180) pm(i) = pm(i) - 360; end
            if (pm(i) < -180) pm(i) = pm(i) + 360; end
        end
		gm = g_margin(sys);
    end
	printf("%10s:\tgcf:%10.3f Hz\tpm:%10.3f deg\tgm:%10.3f\tsse:%10.3f\n",..
		name,bw(tf2ss(sys),0),pm,gm,sse);
endfunction

H.LR_wx = 0.5;
H.FB_wy = 0.5;
H.LRFB_wz = 1;
H.Sum_h = 0.1 + 0.01*%s*20/(%s+20) + 0.01/%s;

openLoopAnalysis("LR->wx",H.LR_wx*sys.oltf(y.wx,u.LR));
openLoopAnalysis("FB->wy",H.FB_wy*sys.oltf(y.wy,u.FB));
openLoopAnalysis("LRFB->wz",H.LRFB_wz*sys.oltf(y.wz,u.LRFB));
openLoopAnalysis("SUM->W",H.Sum_h*sys.oltf(y.h,u.SUM));

// open loop analysis
//scf(1); clf(1);
subplot(1,4,1); bode(H.LR_wx*sys.oltf(x.wx,u.LR),.1,100,"LR->wx");
subplot(1,4,2); bode(H.FB_wy*sys.oltf(x.wy,u.FB),.1,100,"FB->wy");
subplot(1,4,3); bode(H.LRFB_wz*sys.oltf(x.wz,u.LRFB),.1,100,"LRFB->wz");
subplot(1,4,4); bode(H.Sum_h*sys.oltf(x.h,u.SUM),.1,100,"SUM->h");
