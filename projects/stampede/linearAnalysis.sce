mode(-1)

// load constants file
exec constants.sce

// load scicoslab diagram to linearize the dynamics
load stampede.cos

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
	printf("\tclosing loop: %s\n",y.str(yi)+"->"+u.str(ui));
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
	printf('\t\tclbw=%f\tunstable poles=%d/%d\n',..
		bw(ss2cleanTf(sysOut),-3),size(unstablePoles,2),size(abcd(sysOut),1));
endfunction


// extract blocks
disp('extracting blocks for linearization');
dynamics=scs_m.objs(917).model.rpar;
motorLag=scs_m.objs(1061).model.rpar;

// lineriaztion of dynamics
disp('linearizing dynamics');
// vary u to find zero initial conitions
Yd = zeros(15,1)
Yd(y.V) = 2*3; // TODO why is they x 3? 1 m/s velocity
Yd(y.sog) = Yd(y.V);
[Xd,Ud,Yd,XPd] = steadycos2(dynamics,[],[],Yd,[],[1:$],[y.lat,y.lon]);
Xd=clean(Xd,1e-5);
Ud=clean(Ud,1e-5);
ugvTf = clean(ss2tf(lincos(dynamics,Xd,Ud)),1e-5);

// motor lag
motorLagTf = diag([tau_servo/(%s+tau_servo),tau_motor/(%s+tau_motor),0,0]);

sys.oltf = clean(ugvTf,1e-4)*motorLagTf;
sys.olss = minssAutoTol(tf2ss(sys.oltf),16);

// controllers
H.yaw_STR = 0.9*%s/%s; // %s/%s lets scicoslab know this is a tranfer function
H.V_THR = 0.7  + 0.1/%s;

// attitude loops
disp('beginning loop closures');
s = sys.olss;
s0 = ss2cleanTf(s);
[s,u] = closeLoop2(y.yaw,u.STR,s,y,u,H.yaw_STR);
s1 = ss2cleanTf(s);
[s,u] = closeLoop2(y.V,u.THR,s,y,u,H.V_THR);
s2 = ss2cleanTf(s);

//disp('beginning plotting');

// yaw 
f=scf(1); clf(1);
f.figure_size=[600,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));
bode([s0(y.yaw,u.STR);H.yaw_STR*s0(y.yaw,u.STR);s1(y.yaw,u.yaw)],..
	0.01,99,.01,["open loop";"compensated open loop";"compensated closed loop"])
	xs2eps(1,'yaw');

// velocity
f=scf(1); clf(1);
f.figure_size=[600,600];
set_posfig_dim(f.figure_size(1),f.figure_size(2));
bode([s0(y.V,u.THR);H.V_THR*s0(y.V,u.THR);s2(y.V,u.V)],..
	0.01,99,.01,["open loop";"compensated open loop";"compensated closed loop"])
	xs2eps(1,'velocity');

// zoh time effect on pN closed loop
//f=scf(2); clf(2);
//f.figure_size=[600,600];
//set_posfig_dim(f.figure_size(1),f.figure_size(2));
//bode([sPN*pade(4);sPN*pade(2);sPN*pade(1);sPN*pade(1/2);..
	//sPN*pade(1/4);sPN*pade(1/16)],0.01,99,.01,..
	//["1/4 Hz";"1/2 Hz";"1 Hz";"2 Hz";"4 Hz";"16 Hz"])
//xs2eps(2,'pN_closed_zoh');

// zoh time effect on pN open loop
//f=scf(3); clf(3);
//f.figure_size=[600,600];
//set_posfig_dim(f.figure_size(1),f.figure_size(2));
//bode([sPNOpen*pade(4);sPNOpen*pade(2);sPNOpen*pade(1);sPNOpen*pade(1/2);..
	//sPNOpen*pade(1/4);sPNOpen*pade(1/16)],0.01,99,.01,..
	//["1/4 Hz";"1/2 Hz";"1 Hz";"2 Hz";"4 Hz";"16 Hz"])
//xs2eps(3,'pN_open_zoh');
