clc; clear;
mode(-1);

// dependencies
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
m=1;  //kg
Jy=0.1; // guess , moments of inertia
Jz=0.1; // guess
Jx=0.1; // guess

// motor
KV=850 // rpm/Volts 
batVolt=11.1; //Volts
dm=.3; // guess in metres, motor moment arm
tau_motor=18; // guess, motor pole (rad/s)
C_T=0.1; // guess, motor thrust coefficient
C_Q=0.1; // guess, motor torque coefficient

// aerodynamics
rho=1.225; // kg/m^3
rBlade=0.125; // metres
Cd0=0.42; // guess
K_cd_cl=0.02; //guess
s_frame=.1; //guess in m^2
s_frame_side=.1; // guess in m^2

// design controllers

// forward flight
Vt=3; // airspeed, in m/s
// output _ input
Hf.wx_LR 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hf.wy_FB 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hf.wz_LR_FB 	= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hf.phi_wx 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hf.theta_wy 	= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hf.psi_phi 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hf.Vt_Sum 		= 0.4 + 0/%s + 0*%s/(%s+20); 	
Hf.h_theta 		= 0.4 + 0/%s + 0*%s/(%s+20); 	

qwd = quadForwardFlightDesign(Hf);

//hover
U = 0; V = 0; W = 0; // hover
// output _ input
Hh.wx_LR 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hh.wy_FB 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hh.wz_LR_FB 	= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hh.phi_wx 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hh.theta_wy 	= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hh.U_theta 		= 0.5 + 0/%s + 0*%s/(%s+20); 	
Hh.V_phi 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hh.W_Sum 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hh.psi_wz 		= 0.5 + 0/%s + 0*%s/(%s+20); 		
Hh.h_W 			= 0.5 + 0/%s + 0*%s/(%s+20); 	

qhd = quadHoverDesign(Hh);
