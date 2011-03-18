mode(-1)
exec steadycos.sci // load custom steadycos using NDcost
load pixhawk.cos

// zero order hold pade approximation
function sys = pade(controlPeriod)
	sys = (1-%s*controlPeriod/6)/(1 + %s*controlPeriod/3);
endfunction

// continuous pid controller model
function sys = pidCont(kP,kI,kD,controlPeriod)
	sys = syslin('c',kP+kI/%s+%s*kD);
endfunction

// open loop statistics
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

// close a loop
function [sysOut,uOut] = closeLoop(yi,ui,sys,u,H_vect)
	nLoops = max(size(ui));
	for i=1:nLoops
		H(i,i) = H_vect(i,1);
	end
	sysOut = unityFeedback(yi,ui,sys,H);
	uOut = createIndex(y.str(yi),u);
endfunction

// position control loop
PID_POS_INTERVAL = 0.02; // 50 hz
PID_POS_P = 1.7;
PID_POS_I =0.25; 
PID_POS_D = 1.8;
PID_POS_LIM = 0.15;
PID_POS_AWU = 0.1;
PID_POS_Z_P = 0.35;
PID_POS_Z_I = 0.2;
PID_POS_Z_D = 0.15;
PID_POS_Z_LIM = 0.2;
PID_POS_Z_AWU = 3;
VEL_OFFSET_X = 0;
VEL_OFFSET_Y = 0;

// attitude control loop
PID_ATT_INTERVAL = 0.005; // 200 hz
PID_ATT_P= 90;
PID_ATT_I= 60;
PID_ATT_D= 30;
PID_ATT_LIM= 100;
PID_ATT_AWU= 0.3;
PID_YAWPOS_P= 5;
PID_YAWPOS_I= 0.1;
PID_YAWPOS_D= 1;
PID_YAWPOS_LIM= 2;
PID_YAWPOS_AWU= 1;
PID_YAWSPEED_P= 15;
PID_YAWSPEED_I= 5;
PID_YAWSPEED_D= 0;
PID_YAWSPEED_LIM= 20;
PID_YAWSPEED_AWU= 1;
ATT_OFFSET_X =0;
ATT_OFFSET_Y =0;
ATT_OFFSET_Z = -0.080;

// mixing
MIX_REMOTE_WEIGHT = 1;
MIX_POSITION_WEIGHT =1;
MIX_POSITION_Z_WEIGHT = 1;
MIX_POSITION_YAW_WEIGHT = 1;
MIX_OFFSET_WEIGHT =1;

// waypoint
POSITION_SETPOINT_X = 0;
POSITION_SETPOINT_Y = 0;
POSITION_SETPOINT_Z = 0;
POSITION_SETPOINT_YAW = 0.0;

// create indices
r = createIndex(["pN","pE","pD","yaw"]);
m = createIndex(["Vfwd","psi","h","Vside"]);
x = createIndex(["U","W","pitch","pitchRate","V","roll","rollRate","yaw","yawRate","vN","vE","vD","pN","pE","pD"]);
y = x;
u = createIndex(["SUM","FB","LR","LRFB"]);

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

sys.oltf = clean(quadTf*motorLagTf*motorMixTf,1e-5);
sys.olss = minssAutoTol(tf2ss(sys.oltf),16);

H.Pitch_FB = -pidCont(PID_ATT_P,PID_ATT_I,PID_ATT_D,PID_ATT_INTERVAL);
H.Roll_LR = pidCont(PID_ATT_P,PID_ATT_I,PID_ATT_D,PID_ATT_INTERVAL);
H.YawRate_LRFB = pidCont(PID_YAWSPEED_P,PID_YAWSPEED_I,PID_YAWSPEED_D,PID_ATT_INTERVAL);
H.Yaw_YawRate = pidCont(PID_YAWPOS_P,PID_YAWPOS_I,PID_YAWPOS_D,PID_ATT_INTERVAL);
H.pN_Pitch = -0.05*pidCont(PID_POS_P,PID_POS_I,PID_POS_D,PID_POS_INTERVAL);
H.pE_Roll = 0.05*pidCont(PID_POS_P,PID_POS_I,PID_POS_D,PID_POS_INTERVAL);
H.pD_SUM = pidCont(PID_POS_Z_P,PID_POS_Z_I,PID_POS_Z_D,PID_POS_INTERVAL);

// attitude loops
s0 = sys.olss;
[s1,u] = closeLoop([y.pD,y.pitch,y.roll,y.yawRate],..
	[u.SUM,u.FB,u.LR,u.LRFB],s0,u,..
	[H.pD_SUM;H.Pitch_FB;H.Roll_LR;H.YawRate_LRFB]);
[s2,u] = closeLoop(y.yaw,u.yawRate,s1,u,H.Yaw_YawRate);

// position loops
// we can tie in pitch and roll directly since for trim we are aligned with
// North/ East frame
[s3,u] = closeLoop([y.pN;y.pE],..
	[u.pitch;u.roll],..
	s2,u,[H.pN_Pitch;H.pE_Roll]);

openLoopAnalysis("pD->SUM",clean(ss2tf(minss(H.pD_SUM*s1(y.pD,u.SUM)))));
openLoopAnalysis("roll->LR",clean(ss2tf(minss(H.Roll_LR*s1(y.roll,u.LR)))));
openLoopAnalysis("pitch->FB",clean(ss2tf(minss(H.Pitch_FB*s1(y.pitch,u.FB)))));
//openLoopAnalysis("yaw->LRFB",clean(ss2tf(minss(H.YawRate_LRFB*s2(y.yawRate,u.LRFB)))));
//openLoopAnalysis("yawRate->yaw",clean(ss2tf(minss(H.Yaw_YawRate*s2(y.yawRate,u.yaw)))));

openLoopAnalysis("pN->pitch",clean(ss2tf(minss(H.pN_Pitch*s3(y.pN,u.pitch)))));
openLoopAnalysis("pE->roll",clean(ss2tf(minss(H.pE_Roll*s3(y.pE,u.roll)))));

// pull out specific plants
sPitch = ss2tf(s2(y.pitch,u.pitch));
sPN = ss2tf(s3(y.pN,u.pN));
sPNOpen = -ss2tf(s2(y.pN,u.pitch)*H.pN_Pitch);

f=scf(1); clf(1);
f.figure_size=[600,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));
bode([sPitch*pade(PID_ATT_INTERVAL);sPN*pade(PID_POS_INTERVAL)],0.1,99,.01,["pitch";"position north"])
xs2eps(1,'pN_pitch');

f=scf(2); clf(2);
f.figure_size=[600,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));
bode([sPN*pade(4);sPN*pade(2);sPN*pade(1);sPN*pade(1/2);sPN*pade(1/4);sPN*pade(1/16)],0.1,99,.01,..
	["1/4 Hz";"1/2 Hz";"1 Hz";"2 Hz";"4 Hz";"16 Hz"])
xs2eps(2,'pN_closed_zoh');

f=scf(3); clf(3);
f.figure_size=[600,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));
bode([sPNOpen*pade(4);sPNOpen*pade(2);sPNOpen*pade(1);sPNOpen*pade(1/2);sPNOpen*pade(1/4);sPNOpen*pade(1/16)],0.01,99,.01,..
	["1/4 Hz";"1/2 Hz";"1 Hz";"2 Hz";"4 Hz";"16 Hz"])
xs2eps(3,'pN_open_zoh');
