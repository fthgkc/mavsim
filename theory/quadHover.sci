clc; clear;
mode(-1);

function qwd = quad_wind_design

// constants
g=9.81 // m/s^2

// trim conditions
beta=0 // trim
wy=0 //trim
wz=0 //trim
wx=0 //trim
phi=0 //trim
psi=0 // trim
gamma=0 // flight path angle,for level flight is zero
Vt=5 // trim, in m/s

// airframe
m=1 //kg
Jy=1// guess , moments of inertia
Jz=1// guess
Jx=1// guess

// motor
KV=850 // rpm/Volts 
batVolt=11.1 //Volts
dm=.3 // guess in metres, motor moment arm
tau_motor=20 // guess, motor pole (rad/s)
C_T=0.1 // guess, motor thrust coefficient
C_Q=0.1 // guess, motor torque coefficient

// aerodynamics
rho=1.225 // kg/m^3
rBlade=0.125 // metres
Cd0=0.42; // guess
K_cd_cl=0.02 //guess
s_frame=.1 //guess in m^2
s_frame_side=.1 // guess in m^2

// solve for pitch angle at trim;
deff('[y]=theta_sol(x)','y=(-4*%pi^2*rho*x^2*sin(x)*K_cd_cl-rho*sin(x)*Cd0-2*%pi*rho*x*cos(x))*s_frame*Vt^2+(2*cos(x)*sin(x)^2+2*cos(x)^3)*g*m');
theta=fsolve(-10*%pi/180,theta_sol)
alpha=theta;
qwd.trim.theta=theta;

// solve for T_sumSq at trim
//check the equation below; there was a minus sign in front
T_sumSq_trim= ((1800*%pi^2*rho*theta^2*K_cd_cl+450*rho*Cd0)*..
s_frame*Vt^2)/(%pi^3*cos(theta)*rho*batVolt^2*KV^2*rBlade^4*C_T)
qwd.trim.T_sumSq = T_sumSq_trim;
T_sumSq=T_sumSq_trim;
dutycycle_Lc=T_sumSq/4;
dutycycle_L=T_sumSq/4;
dutycycle_Rc=T_sumSq/4;
dutycycle_R=T_sumSq/4;
dutycycle_Bc=T_sumSq/4;
dutycycle_B=T_sumSq/4;
dutycycle_Fc=T_sumSq/4;
dutycycle_F=T_sumSq/4;

// include dynamics
exec quad_wind_dynamics.sci;

qwd.open.ss = syslin('c',F_wind_quad,G_wind_quad,C_wind_quad,D_wind_quad);
qwd.open.tf = clean(ss2tf(qwd.open.ss),1e-8);

// state
qwd.names.x(1) = "Vt";
qwd.names.x(2) = "alpha";
qwd.names.x(3) = "theta";
qwd.names.x(4) = "wy";
qwd.names.x(5) = "h";
qwd.names.x(6) = "beta";
qwd.names.x(7) = "phi";
qwd.names.x(8) = "wx";
qwd.names.x(9) = "psi";
qwd.names.x(10) = "wz";
qwd.names.x(11) = "dcL";
qwd.names.x(12) = "dcR";
qwd.names.x(13) = "dcF";
qwd.names.x(14) = "dcB";

// output
qwd.names.y(1) = "Vt";
qwd.names.y(2) = "theta";
qwd.names.y(3) = "wy";
qwd.names.y(4) = "h";
qwd.names.y(5) = "phi";
qwd.names.y(6) = "wx";
qwd.names.y(7) = "psi";
qwd.names.y(8) = "wz";

// input
qwd.names.u(1) = "sum_sq";
qwd.names.u(2) = "F_B_sq";
qwd.names.u(3) = "L_R_sq";
qwd.names.u(4) = "RL_FB_sq";

// include unityFeedback function
exec unityFeedback.sci;

disp("Closing Loop 1");
qwd.Loop(1).H = diag([
		-(0.01 + 0.5/%s + 0.5*%s/(%s+20)); 	// altitude error to power
		-(0.5 + 0/%s + 0*%s/(%s+20)); 	// roll rate error to lf
		0.5 + 0/%s + 0*%s/(%s+20); 	// pitch rate error to fb
		0.5 + 0/%s + 0*%s/(%s+20)  	// yaw rate error to fb_lf
]); 
qwd.Loop(1).u = [1,3,2,4];
qwd.Loop(1).y = [4,6,3,8];
qwd.Loop(1).olss=qwd.open.ss;
qwd.Loop(1).oltf=clean(ss2tf(qwd.Loop(1).olss));
qwd.Loop(1).clss = unityFeedback(qwd.Loop(1).olss,qwd.Loop(1).H,qwd.Loop(1).y,qwd.Loop(1).u);
qwd.Loop(1).cltf = clean(ss2tf(qwd.Loop(1).clss)); 
qwd.names.u(5) = "altitude command";
qwd.names.u(6) = "roll rate command";
qwd.names.u(7) = "pitch rate command";
qwd.names.u(8) = "yaw rate command";
scf(1); clf(1);
subplot(1,4,1); bode(qwd.Loop(1).cltf(4,5),.01,100,.1,qwd.names.u(5));
subplot(1,4,2); bode(qwd.Loop(1).cltf(6,6),.01,100,.1,qwd.names.u(6));
subplot(1,4,3); bode(qwd.Loop(1).cltf(3,7),.01,100,.1,qwd.names.u(7));
subplot(1,4,4); bode(qwd.Loop(1).cltf(8,8),.01,100,.1,qwd.names.u(8));
disp("Loop 1 Closed");

disp("Closing Loop 2");
qwd.Loop(2).H = diag([
		0.5 + 0/%s + 0*%s/(%s+20); 	// roll error to roll rate command
		0.5 + 0.1/%s + 0*%s/(%s+20); 	// velocity error to pitch rate command
		0.5 + 0/%s + 0*%s/(%s+20)  	// yaw error to yaw rate command
]); 
qwd.Loop(2).u = [6,7,8]; 
qwd.Loop(2).y = [5,1,7];
qwd.Loop(2).olss=qwd.Loop(1).clss;
qwd.Loop(2).oltf=qwd.Loop(1).cltf;
qwd.Loop(2).clss = unityFeedback(qwd.Loop(1).clss,qwd.Loop(2).H,qwd.Loop(2).y,qwd.Loop(2).u);
qwd.Loop(2).cltf = clean(ss2tf(qwd.Loop(2).clss)); 
qwd.names.u(9) = "roll command";
qwd.names.u(10) = "velocity command";
qwd.names.u(11) = "yaw command";
scf(2); clf(2);
subplot(1,4,1); bode(qwd.Loop(2).cltf(5,9),.01,100,.1,qwd.names.u(9));
subplot(1,4,2); bode(qwd.Loop(2).cltf(2,10),.01,100,.1,qwd.names.u(10));
subplot(1,4,3); bode(qwd.Loop(2).cltf(7,11),.01,100,.1,qwd.names.u(11));
disp("Loop 2 Closed");

// plots
//scf(1);

// wx
//sys.i = 2; sys.j = 2; sys.name = "wx rate";
//sys.open.ss=open.ss(6,3);
//sys.open.tf=clean(ss2tf(sys.open.ss),1e-8);
//sys.closed.ss = closed.ss(sys.i,sys.j);
//sys.closed.tf = clean(ss2tf(sys.closed.ss));
//sys.phase_margin = min(180 + p_margin(sys.closed.ss));
//sys.gain_margin = g_margin(sys.closed.ss);
//wx=sys;

// plots
//sys = wx; disp(sys.name); disp(sys); if (sys.closed.tf ~= 0) subplot(1,3,sys.i); bode(sys.closed.ss,.01,1000,.1,sys.name); end
//sys = wz; disp(sys.name); disp(sys); if (sys.closed.tf ~= 0) subplot(1,3,sys.i); bode(sys.closed.ss,.01,1000,.1,sys.name); end
endfunction

qwd= quad_wind_design();
