clc; clear;
mode(-1);

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

// value left to compute
dutycycle_Lc=0.5
dutycycle_L=0.5
dutycycle_Rc=0.5
dutycycle_R=0.5
dutycycle_Bc=0.5
dutycycle_B=0.5
dutycycle_Fc=0.5
dutycycle_F=0.5
T_sumSq=1.0 // guess in N

// solve for pitch angle at trim
deff('[y]=theta_sol(x)','y=(-4*%pi^2*rho*x^2*sin(x)*K_cd_cl-rho*sin(x)*Cd0-2*%pi*rho*x*cos(x))*s_frame*Vt^2+(2*cos(x)*sin(x)^2+2*cos(x)^3)*g*m');
theta=fsolve(-10*%pi/180,theta_sol)
alpha=theta;

// solve for T_sumSq at trim
T_sumSq_trim=-((1800*%pi^2*rho*theta^2*K_cd_cl+450*rho*Cd0)*..
s_frame*Vt^2)/(%pi^3*cos(theta)*rho*batVolt^2*KV^2*rBlade^4*C_T)

// include dynamics
exec quad_wind_dynamics.sci;

quad_wind_dynamics.ss = syslin('c',F_wind_quad,G_wind_quad,C_wind_quad,D_wind_quad);
quad_wind_dynamics.tf = clean(ss2tf(quad_wind_dynamics.ss),1e-8);
quad_wind_dynamics.names.x = ['Vt','alpha','theta''wy','h','beta','phi','wx','psi','wz','dcL','dcR','dcF','dcB'];
quad_wind_dynamics.names.y = ['Vt','theta','wy','h','phi','wx','psi','wz'];
quad_wind_dynamics.names.u = ['sum_sq','F_B_sq','L_R_sq','RL_FB_sq'];

// include unityFeedback function
exec unityFeedback.sce;

// renaming for convenience
open = quad_wind_dynamics;

// close wy, wx, wz loops with FB/ LR/ LR_FB inputs
H1.tf = diag([
		0.2 + 0/%s + .1*%s/(%s+20); 	// wy, pid with low pass on deriv
		0.2 + 0/%s + .1*%s/(%s+20); 	// wx, pid with low pass on deriv
		0.2 + 0/%s + .1*%s/(%s+20)  	// wz, pid with low pass on deriv
]); 

H1.ss = tf2ss(H1.tf);
closed.ss = unityFeedback(open.ss([3,6,8],[2,3,4]),H1.ss);
closed.tf = clean(ss2tf(closed.ss),1e-8);

// plots
scf(1);

// wy
sys.i = 1; sys.j = 1; sys.name = "wy rate";
sys.open.ss=open.ss(6,3);
sys.open.tf=clean(ss2tf(sys.open.ss),1e-8);
sys.closed.ss = closed.ss(sys.i,sys.j);
sys.closed.tf = clean(ss2tf(sys.closed.ss));
sys.phase_margin = min(180 + p_margin(sys.closed.ss));
sys.gain_margin = g_margin(sys.closed.ss);
wy=sys;

// wx
sys.i = 2; sys.j = 2; sys.name = "wx rate";
sys.open.ss=open.ss(6,3);
sys.open.tf=clean(ss2tf(sys.open.ss),1e-8);
sys.closed.ss = closed.ss(sys.i,sys.j);
sys.closed.tf = clean(ss2tf(sys.closed.ss));
sys.phase_margin = min(180 + p_margin(sys.closed.ss));
sys.gain_margin = g_margin(sys.closed.ss);
wx=sys;

// wz
sys.i = 3; sys.j = 3; sys.name = "wz rate";
sys.open.ss=open.ss(6,3);
sys.open.tf=clean(ss2tf(sys.open.ss),1e-8);
sys.closed.ss = closed.ss(sys.i,sys.j);
sys.closed.tf = clean(ss2tf(sys.closed.ss));
sys.phase_margin = min(180 + p_margin(sys.closed.ss));
sys.gain_margin = g_margin(sys.closed.ss);
wz=sys;

// plots
sys = wy; disp(sys.name); disp(sys); if (sys.closed.tf ~= 0) subplot(1,3,sys.i); bode(sys.closed.ss,.01,1000,.1,sys.name); end
sys = wx; disp(sys.name); disp(sys); if (sys.closed.tf ~= 0) subplot(1,3,sys.i); bode(sys.closed.ss,.01,1000,.1,sys.name); end
sys = wz; disp(sys.name); disp(sys); if (sys.closed.tf ~= 0) subplot(1,3,sys.i); bode(sys.closed.ss,.01,1000,.1,sys.name); end

