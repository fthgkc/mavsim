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

// aerodynamics
rho=1.225; // kg/m^3
rBlade=0.125; // metres
Cd0=0.42; // guess
K_cd_cl=0.02; //guess
s_frame=.1; //guess in m^2
s_frame_side=.1; // guess in m^2

// motor
KV=850 // rpm/Volts 
batVolt=11.1; //Volts
dm=.3; // guess in metres, motor moment arm
tau_motor=18; // guess, motor pole (rad/s)
T_max = 5; // max motor thrust in newtons
torque_max = 10; // max motor thrust in newton-m
C_T = T_max / (rho*%pi*rBlade^4*(KV*2*%pi/60*batVolt)^2);
C_Q = torque_max / (rho*%pi*rBlade^4*(KV*2*%pi/60*batVolt)^2);

// design controllers

// forward flight
Vt=3; // airspeed, in m/s
// output _ input
Hf.wx_LR 		= 0.05 + 0/%s + 0*%s/(%s+20); 		
Hf.wy_FB 		= 0.0 + 0/%s + 0*%s/(%s+20); 		
Hf.wz_LR_FB 	= 0.0 + 0/%s + 0*%s/(%s+20); 		
Hf.phi_wx 		= 0 + 0/%s + 0*%s/(%s+20); 		
Hf.theta_wy 	= 0 + 0/%s + 0*%s/(%s+20); 		
Hf.psi_phi 		= 0 + 0/%s + 0*%s/(%s+20); 		
Hf.Vt_Sum 		= -(0.0 + 0/%s + 0*%s/(%s+20)); 	
Hf.h_theta 		= 0 + 0/%s + 0*%s/(%s+20); 	

qwd = quadForwardFlightDesign(Hf);

//hover
U = 0; V = 0; W = 0; // hover
// output _ input
Hh.wx_LR 		= 0.5*(%s+18); // 30 Hz	
Hh.wy_FB 		= 0.5*(%s+18); // 30 Hz
Hh.wz_LR_FB 	= 1.0*(%s+18); // 30 Hz		
Hh.W_Sum 		= -2.1*(%s+18); // 30 Hz	
Hh.phi_wx 		= 18.0; 		
Hh.theta_wy 	= 0.0; 		
Hh.psi_wz 		= 0.0; 		
Hh.U_theta 		= 0.0; 	
Hh.V_phi 		= 0.0; 		
Hh.h_W 			= 0.0; 	

qhd = quadHoverDesign(Hh);
