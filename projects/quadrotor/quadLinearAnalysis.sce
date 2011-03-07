mode(-1)
exec steadycos.sci // load custom steadycos using NDcost
load arducopter.cos
x = createIndex(["U","W","theta","wy","h","V","phi","wx","psi","wz","LR","FB","LRFB","SUM"]);
y = createIndex(["U","W","theta","wy","h","V","phi","wx","psi","wz","LR","FB","LRFB","SUM"]);
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
sys.olss = minssAutoTol(tf2ss(sys.oltf),14);

function openLoopAnalysis(name,sys)
	printf("%10s:\tgcf:%10.3f Hz\tpm:%10.3f deg\tgm:%10.3f\n",name,bw(sys,0),p_margin(sys),g_margin(sys));
endfunction

openLoopAnalysis("LR->wx",sys.olss(y.wx,u.LR));
openLoopAnalysis("FB->wy",sys.olss(y.wy,u.FB));
openLoopAnalysis("LRFB->wz",sys.olss(y.wz,u.LRFB));
openLoopAnalysis("SUM->W",sys.olss(y.W,u.SUM));


// open loop analysis
scf(1); clf(1);
subplot(1,3,1); bode(sys.oltf(x.wx,u.LR),.1,100,"LR->wx");
subplot(1,4,2); bode(sys.oltf(x.wy,u.FB),.1,100,"FB->wy");
subplot(1,4,3); bode(sys.oltf(x.wz,u.LRFB),.1,100,"LRFB->wz");
subplot(1,4,4); bode(sys.oltf(x.W,u.SUM),.1,100,"SUM->W");
