mode(-1)

// load constants file
exec constants.sce

// load scicoslab diagram to linearize the dynamics
load cheetah.cos

function tf = ss2cleanTf(ss)
	tf = clean(ss2tf(ss));
endfunction

// open loop statistics
function openLoopAnalysis(sys)
	if(typeof(sys)=='state-space') sys = ss2cleanTf(sys); end
	sse=1/(horner(sys,1e-10));
	if (sse>1e6) sse=%inf; end
    printf('\t\tgcf=%8.2f Hz\t\tsse=%8.2f\n',bw(tf2ss(sys),0),sse);
endfunction

// close a loop
function [sysOut,uOut] = closeLoop2(yi,ui,sys,y,u,H)
	printf('\tclosing loop: %s\n',y.str(yi)+'->'+u.str(ui));
	openLoopAnalysis(H*sys(yi,ui));
	sysOut = unityFeedback2(yi,ui,sys,H);
	uOut = createIndex(y.str(yi),u);
	[eVect,eVal] = spec(abcd(sysOut));
	eVal = diag(eVal);
	unstablePoles=find(real(eVal)>0);
	printf('\t\tunstable modes:\n');
	for i=1:size(unstablePoles,2)
		[junk,k]=sort(eVect(:,unstablePoles(i)));
		j=0; // number of valid states found
		m=1; // index 
		printf('\t\t\t');
		while 1
			if (k(m)<=size(y.str,1))		
				j = j +1;
				printf('%9s\t',y.str(k(m)));
			end
			if (j>2)
				printf('\t%8.3f + %8.3f j\n',..
					real(eVal(unstablePoles(i))),..
					imag(eVal(unstablePoles(i))));
				break;
			else
				m = m +1;
			end;
		end
	end
	poles=size(abcd(sys),1);
	printf('\t\tunstable poles=%d/%d\n',size(unstablePoles,2),size(abcd(sysOut),1));
endfunction


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
s1 = sys.oltf;
i=0;

// set default figure
f = gdf();
f.color_map(8,:) = [0,0,0]; // set white to black in color map so it can be seen

// yawRate 
[s,u] = closeLoop2(y.yawRate,u.LRFB,s,y,u,H.yawRate_LRFB);
i = i+1;
s0 = s1; 
s1 = ss2cleanTf(s);

// yawRate design plots
f=scf(i); clf(i);
f.figure_name='yawRateg';
set_posfig_dim(1200,600);

subplot(1,3,1)
bode([s0(y.yawRate,u.LRFB);H.yawRate_LRFB*s0(y.yawRate,u.LRFB);s1(y.yawRate,u.yawRate)],..
	0.01,99,.01,['open loop';'compensated open loop';'compensated closed loop'])

subplot(1,3,2)
evans(s0(y.yawRate,u.LRFB),100);
title(gca(),'Uncompensated Root Locus');
mtlb_axis([-10,10,-10,10]);

subplot(1,3,3)
evans(H.yawRate_LRFB*s0(y.yawRate,u.LRFB),10);
title(gca(),'Compensated Root Locus');
mtlb_axis([-10,10,-10,10]);

xs2eps(i,'yawRate');

// pD
[s,u] = closeLoop2(y.pD,u.SUM,s,y,u,H.pD_SUM);
i = i+1;
s0 = s1; 
s1 = ss2cleanTf(s);

// pD design plots
f=scf(i); clf(i);
f.figure_name='pD';
f.figure_size = [1200,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));

subplot(1,3,1)
bode([-s0(y.pD,u.SUM);H.pD_SUM*s0(y.pD,u.SUM);s1(y.pD,u.pD)],..
	0.01,99,.01,['open loop';'compensated open loop';'compensated closed loop'])

subplot(1,3,2)
evans(-s0(y.pD,u.SUM),100);
title(gca(),'Uncompensated Root Locus');
mtlb_axis([-10,10,-10,10]);

subplot(1,3,3)
evans(H.pD_SUM*s0(y.pD,u.SUM),10);
title(gca(),'Compensated Root Locus');
mtlb_axis([-10,10,-10,10]);

xs2eps(i,'pD');

// roll 
[s,u] = closeLoop2(y.roll,u.LR,s,y,u,H.roll_LR);
i = i+1;
s0 = s1; 
s1 = ss2cleanTf(s);

// roll design plots
f=scf(i); clf(i);
f.figure_name='roll';
f.figure_size = [1200,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));

subplot(1,3,1);
bode([s0(y.roll,u.LR);H.roll_LR*s0(y.roll,u.LR);s1(y.roll,u.roll)],..
	0.01,99,.01,['open loop';'compensated open loop';'compensated closed loop'])

subplot(1,3,2)
evans(s0(y.roll,u.LR),100);
title(gca(),'Uncompensated Root Locus');
mtlb_axis([-10,10,-10,10]);

subplot(1,3,3)
evans(H.roll_LR*s0(y.roll,u.LR),10);
title(gca(),'Compensated Root Locus');
mtlb_axis([-10,10,-10,10]);

xs2eps(i,'roll');

// pitch 
[s,u] = closeLoop2(y.pitch,u.FB,s,y,u,H.pitch_FB);
i = i+1;
s0 = s1; 
s1 = ss2cleanTf(s);

f=scf(i); clf(i);
f.figure_name='pitch';
f.figure_size = [1200,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));

subplot(1,3,1);
bode([s0(y.pitch,u.FB);H.pitch_FB*s0(y.pitch,u.FB);s1(y.pitch,u.pitch)],..
	0.01,99,.01,['open loop';'compensated open loop';'compensated closed loop'])

subplot(1,3,2)
evans(s0(y.pitch,u.FB),100);
title(gca(),'Uncompensated Root Locus');
mtlb_axis([-10,10,-10,10]);

subplot(1,3,3)
evans(H.pitch_FB*s0(y.pitch,u.FB),10);
title(gca(),'Compensated Root Locus');
mtlb_axis([-10,10,-10,10]);

xs2eps(i,'pitch');

// yaw 
i = i+1;
[s,u] = closeLoop2(y.yaw,u.yawRate,s,y,u,H.yaw_yawRate);
s0 = s1; 
s1 = ss2cleanTf(s);

f=scf(i); clf(i);
f.figure_name='yaw';
f.figure_size = [1200,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));

subplot(1,3,1)
bode([s0(y.yaw,u.yawRate);H.yaw_yawRate*s0(y.yaw,u.yawRate);s1(y.yaw,u.yaw)],..
	0.01,99,.01,['open loop';'compensated open loop';'compensated closed loop'])

subplot(1,3,2)
evans(s0(y.yaw,u.yawRate),100);
title(gca(),'Uncompensated Root Locus');
mtlb_axis([-10,10,-10,10]);

subplot(1,3,3)
evans(H.yaw_yawRate*s0(y.yaw,u.yawRate),10);
title(gca(),'Compensated Root Locus');
mtlb_axis([-10,10,-10,10]);

xs2eps(i,'yaw');

// position loops
// we can tie in pitch and roll directly since for trim we are aligned with
// North/ East frame

// pN 
i = i+1;
[s,u] = closeLoop2(y.pN,u.pitch,s,y,u,H.pN_pitch);
s0 = s1; 
s1 = ss2cleanTf(s);

sPN = s1(y.pN,u.pN);
sPNOpen = -s0(y.pN,u.pitch);

f=scf(i); clf(i);
f.figure_name='pN';
f.figure_size = [1200,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));

subplot(1,3,1)
bode([-s0(y.pN,u.pitch);H.pN_pitch*s0(y.pN,u.pitch);s1(y.pN,u.pN)],..
	0.01,99,.01,['open loop';'compensated open loop';'compensated closed loop'])

subplot(1,3,2)
evans(-s0(y.pN,u.pitch),100);
title(gca(),'Uncompensated Root Locus');
mtlb_axis([-10,10,-10,10]);

subplot(1,3,3)
evans(H.pN_pitch*s0(y.pN,u.pitch),10);
title(gca(),'Compensated Root Locus');
mtlb_axis([-10,10,-10,10]);

xs2eps(i,'pN');

// pE 
i = i+1;
[s,u] = closeLoop2(y.pE,u.roll,s,y,u,H.pE_roll);
s0 = s1; 
s1 = ss2cleanTf(s);

f=scf(i); clf(i);
f.figure_name='pE';
f.figure_size = [1200,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));

subplot(1,3,1)
bode([s0(y.pE,u.roll);H.pE_roll*s0(y.pE,u.roll);s1(y.pE,u.pE)],..
	0.01,99,.01,['open loop';'compensated open loop';'compensated closed loop'])

subplot(1,3,2)
evans(s0(y.pE,u.roll),100);
title(gca(),'Uncompensated Root Locus');
mtlb_axis([-10,10,-10,10]);

subplot(1,3,3)
evans(H.pE_roll*s0(y.pE,u.roll),10);
title(gca(),'Compensated Root Locus');
mtlb_axis([-10,10,-10,10]);

xs2eps(i,'pE');

// zoh time effect on pN closed loop
i = i +1;
f=scf(i); clf(i);
f.figure_name='pN zero order hold effect, closed loop';
f.figure_size = [800,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));

bode([sPN*pade(4);sPN*pade(2);sPN*pade(1);sPN*pade(1/2);..
	sPN*pade(1/4);sPN*pade(1/16)],0.01,99,.01,..
	['1/4 Hz';'1/2 Hz';'1 Hz';'2 Hz';'4 Hz';'16 Hz'])
xs2eps(2,'pN_closed_zoh');

// zoh time effect on pN open loop
i = i +1;
f=scf(i); clf(i);
f.figure_name='pN zero order hold effect, open loop';
f.figure_size = [800,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));

bode([sPNOpen*pade(4);sPNOpen*pade(2);sPNOpen*pade(1);sPNOpen*pade(1/2);..
	sPNOpen*pade(1/4);sPNOpen*pade(1/16)],0.01,99,.01,..
	['1/4 Hz';'1/2 Hz';'1 Hz';'2 Hz';'4 Hz';'16 Hz'])
xs2eps(3,'pN_open_zoh');
