mode(-1)

// load constants file
exec constants.sce

// load scicoslab diagram to linearize the dynamics
load stampede.cos

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

// initializiation
disp('beginning loop closures');
s = sys.olss;
s1 = sys.oltf;
i = 0;

// set default figure
f = gdf();
f.color_map(8,:) = [0,0,0]; // set white to black in color map so it can be seen

// yaw
[s,u] = closeLoop2(y.yaw,u.STR,s,y,u,H.yaw_STR);
i = i+1; 
s0 = s1;
s1 = ss2cleanTf(s);

// yaw design plots




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
