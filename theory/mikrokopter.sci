clc; clear;

mode(-1);

// load dependencies
exec utilities.sci;
exec quadForwardFlightDesign.sci;
exec quadHoverDesign.sci;

// constants
g=9.81; // m/s^2

// trim conditions
beta=0; // trim
wy=0; //trim
wz=0; //trim
wx=0; //trim
phi=0; //trim
psi=0; // trim
gamma=0; // flight path angle,for level flight is zero
// airframe
m=1.02; //kg
Jy=0.1; // guess , moments of inertia
Jz=0.1; // guess
Jx=0.1; // guess

// motor
KV=760; // rpm/Volts 
batVolt=14.8; //Volts
dm=.25; // guess in metres, motor moment arm
tau_motor=36.95991; // guess, motor pole (rad/s)
C_T=5.17e-3; // guess, motor thrust coefficient
C_Q=0.025; // guess, motor torque coefficient

// aerodynamics
rho=1.225; // kg/m^3
rBlade=0.127; // metres
Cd0=0.42; // guess
K_cd_cl=0.02; //guess
s_frame=.1; //guess in m^2
s_frame_side=.1; // guess in m^2

// design controllers

// forward flight
Vt=3; // airspeed, in m/s
// output _ input
Hf.wx_LR 		= (%s+tau_motor);
Hf.wy_FB 		= (%s+tau_motor);
Hf.wz_LR_FB 	= (%s+tau_motor);
Hf.phi_wx 		= 0;
Hf.theta_wy 	= 0;
Hf.psi_phi 		= 0;
Hf.Vt_Sum 		= (%s+tau_motor);
Hf.h_theta 		= 0;

qwd = quadForwardFlightDesign(Hf);

//hover
U = 0; V = 0; W = 0; // hovering 
// output _ input
Hh.wx_LR 		= 0.1*(%s+tau_motor);
Hh.wy_FB 		= 0; 
Hh.wz_LR_FB 	= 0;
Hh.phi_wx 		= 0;
Hh.theta_wy 	= 0;
Hh.U_theta 		= 0;
Hh.V_phi 		= 0;
Hh.W_Sum 		= 0;
Hh.psi_wz 		= 0;
Hh.h_W 			= 0;

qhd = quadHoverDesign(Hh);
