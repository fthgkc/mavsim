mode(-1)

// load constants file
exec mikrokopterConstants.sce

// load scicoslab diagram to linearize the dynamics
load mikrokopter.cos

// open loop statistics
function openLoopAnalysis(name,sys)
	if(typeof(sys)=="state-space") sys=clean(ss2tf(minss(sys,1e-100))); end
	//disp("sys:"); disp(sys)
	//sse
	sse=1/(horner(sys,1e-10));

	if (sse>1e6) sse=%inf; end

	// stability
    unstablePoles=size(find(real(spec(abcd(sys)))>0),2)
	poles=size(abcd(sys));

	printf("%10s:\tgcf=%8.2f Hz\t\tsse=%8.2f\t\tunst poles=%d/%d\n",..
		name,bw(tf2ss(sys),0),sse,unstablePoles,poles);
endfunction

// close a loop
function [sysOut,uOut] = closeLoop2(yi,ui,sys,y,u,H)
	openLoopAnalysis(y.str(yi)+"->"+u.str(ui),H*sys(yi,ui));
	sysOut = unityFeedback2(yi,ui,sys,H);
	uOut = createIndex(y.str(yi),u);
endfunction

// extract blocks
dynamics=scs_m.objs(1).model.rpar;
controller=scs_m.objs(299).model.rpar;
motorLag=scs_m.objs(96).model.rpar;
motorMix=scs_m.objs(209).model.rpar;
navigation=scs_m.objs(390).model.rpar;

// lineriaztion of dynamics

// vary u to find zero initial conitions
[Xd,Ud,Yd,XPd] = steadycos(dynamics,[],[],[],[],[1:$],[],[]);
Xd=clean(Xd,1e-5);
Ud=clean(Ud,1e-5);
quadTf = clean(ss2tf(lincos(dynamics,Xd,Ud)),1e-5);

// motor mix block
motorMixTf = clean(ss2tf(lincos(motorMix,[],[Ud(1,1)*255;0;0;0])),1e-5);

// motor lag block
motorLagTf = clean(ss2tf(lincos(motorLag,zeros(4,1),Ud)),1e-5);

// navigatoin block
//[Xnav,Unav]=steadycos(navigation,[],[],[],[],[],[],[]);
//navTf = clean(ss2tf(lincos(navigation,Xnav,Unav)),1e-5);

sys.oltf = clean(quadTf*motorLagTf*motorMixTf,1e-4);
sys.olss = minssAutoTol(tf2ss(sys.oltf),16);

// attitude loops
s = sys.oltf;
s0 = s;
[s,u] = closeLoop2(y.yawRate,u.LRFB,s,y,u,H.YawRate_LRFB);
s1 = s;
[s,u] = closeLoop2(y.roll,u.LR,s,y,u,H.Roll_LR);
s2 = s;
[s,u] = closeLoop2(y.pitch,u.FB,s,y,u,H.Pitch_FB);
s3 = s;
[s,u] = closeLoop2(y.yaw,u.yawRate,s,y,u,H.Yaw_YawRate);
s4 = s;

sPitch = clean(ss2tf(s4(y.pitch,u.pitch)));
sPNOpen = clean(ss2tf(s4(y.pN,u.pitch)*H.pN_Pitch));

// position loops
// we can tie in pitch and roll directly since for trim we are aligned with
// North/ East frame
[s,u] = closeLoop2(y.pD,u.SUM,s,y,u,H.pD_SUM);
s5 = s;
[s,u] = closeLoop2(y.pN,u.pitch,s,y,u,H.pN_Pitch);
s6 = s;
[s,u] = closeLoop2(y.pE,u.roll,s,y,u,H.pE_Roll);
s7 = s;
sPN = clean(ss2tf(s7(y.pN,u.pN)));

// position north, and pitch
f=scf(1); clf(1);
f.figure_size=[600,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));
bode([sPitch*pade(PID_ATT_INTERVAL);sPN*pade(PID_POS_INTERVAL)],..
	0.01,99,.01,["pitch";"position north"])
xs2eps(1,'pN_pitch');

// zoh time effect on pN closed loop
f=scf(2); clf(2);
f.figure_size=[600,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));
bode([sPN*pade(4);sPN*pade(2);sPN*pade(1);sPN*pade(1/2);..
	sPN*pade(1/4);sPN*pade(1/16)],0.01,99,.01,..
	["1/4 Hz";"1/2 Hz";"1 Hz";"2 Hz";"4 Hz";"16 Hz"])
xs2eps(2,'pN_closed_zoh');

// zoh time effect on pN open loop
f=scf(3); clf(3);
f.figure_size=[600,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));
bode([sPNOpen*pade(4);sPNOpen*pade(2);sPNOpen*pade(1);sPNOpen*pade(1/2);..
	sPNOpen*pade(1/4);sPNOpen*pade(1/16)],0.01,99,.01,..
	["1/4 Hz";"1/2 Hz";"1 Hz";"2 Hz";"4 Hz";"16 Hz"])
xs2eps(3,'pN_open_zoh');
