mode(-1)

// load constants file
exec constants.sce

// load scicoslab diagram to linearize the dynamics
load cheetah.cos

// extract blocks
disp('extracting blocks for linearization');
dynamics=scs_m.objs(1).model.rpar;
controller=scs_m.objs(299).model.rpar;
motorLag=scs_m.objs(96).model.rpar;
motorMix=scs_m.objs(209).model.rpar;
navigation=scs_m.objs(390).model.rpar;

// lineriaztion of dynamics
disp('linearizing dynamics');
// vary u to find zero initial conitions
[Xd,Ud,Yd,XPd] = steadycos2(dynamics,[],[],[],[],[1:$],[],[]);
Xd=clean(Xd,1e-5);
Ud=clean(Ud,1e-5);
quadTf = clean(ss2tf(lincos(dynamics,Xd,Ud)),1e-5);

// motor mix block
disp('linearizing motor mix block');
motorMixTf = clean(ss2tf(lincos(motorMix,[],[Ud(1)*255;0;0;0])),1e-5);

// motor lag block
disp('linearizing motor lag block');
motorLagTf = clean(ss2tf(lincos(motorLag,zeros(4,1),Ud)),1e-5);

// find complete dynamics transfer function
disp('finding dynamics transfer function');
sys.oltf = clean(quadTf*motorLagTf*motorMixTf,1e-4);
sys.olss = minssAutoTol(tf2ss(sys.oltf),16);

// attitude loops
disp('beginning loop closures');
s = sys.olss;
i= 1;

// attitude loops
[f,s,u,i] = closeLoopWithPlots('yawRate',i,y.yawRate,u.LRFB,s,y,u,H.yawRate_LRFB);
[f,s,u,i] = closeLoopWithPlots('pD',i,y.pD,u.SUM,s,y,u,H.pD_SUM);
[f,s,u,i] = closeLoopWithPlots('roll',i,y.roll,u.LR,s,y,u,H.roll_LR);
[f,s,u,i] = closeLoopWithPlots('pitch',i,y.pitch,u.FB,s,y,u,H.pitch_FB);
[f,s,u,i] = closeLoopWithPlots('yaw',i,y.yaw,u.yawRate,s,y,u,H.yaw_yawRate);

// position loops
// we can tie in pitch and roll directly since for trim we are aligned with
// North/ East frame at the linearization point
sPNOpen = H.pN_pitch*s(y.pN,u.pitch);
[f,s,u,i] = closeLoopWithPlots('pN',i,y.pN,u.pitch,s,y,u,H.pN_pitch);
sPNClosed = s(y.pN,u.pN);
[f,s,u,i] = closeLoopWithPlots('pE',i,y.pE,u.roll,s,y,u,H.pE_roll);

// zoh time effect on pN closed loop
[f,i] = zohAnalysisPlot('pN',i, sPNOpen, sPNClosed, [.25, .5, 1, 2, 4, 16]);

// step responses
load cheetahBatch.cos

// manual input set to zero
mSignal = struct();
mSignal.time = 0;
mSignal.values = zeros(1,4);

// solver settings
scs_m.props.tf = 15;

// position step responses
stepAnalysis(s,['pN';'pE';'pD'],[0.1 1],['pN, meters'; 'pE, meters'; 'pD, meters'],y,u,r);
