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
Hf(1,1) = 1 + 0/%s + .05*%s/(%s+20); 		// LR -> wx
Hf(2,1) = 1 + 0/%s + .05*%s/(%s+20); 		// FB -> wy
Hf(3,1) = 0.5 + 1.0/%s + 0*%s/(%s+20); 		// wx -> phi
Hf(4,1) = 0.1 + 1.0/%s + 0*%s/(%s+20); 		// wy -> theta
Hf(5,1) = .1 + 0/%s + .1*%s/(%s+20); 		// Sum -> Vt
Hf(6,1) = 0.0 + 1.0/%s + 0*%s/(%s+20); 		// phi -> psi
Hf(7,1) = 0.01 + 0.2/%s + 0*%s/(%s+20); 	// theta -> h
qwd = quadForwardFlightDesign(Hf);

//hover
U = 0; V = 0; W = 0; // hover
Hh(1,1) = 0.5 + 0/%s + 0*%s/(%s+20); // LR -> wx
Hh(2,1) = 0.5 + 0/%s + 0*%s/(%s+20); // FB -> wy
Hh(3,1) = 0.5 + 0/%s + 0*%s/(%s+20); // wx -> phi
Hh(4,1) = 0.5 + 0/%s + 0*%s/(%s+20); // wy -> theta
Hh(5,1) = 0.5 + 0/%s + 0*%s/(%s+20); // theta -> U
Hh(6,1) = 0.5 + 0/%s + 0*%s/(%s+20); // phi -> V
Hh(7,1) = 0.5 + 0/%s + 0*%s/(%s+20); // Sum -> W
Hh(8,1) = 0.5 + 0/%s + 0*%s/(%s+20); // LR_FB -> psi
Hh(9,1) = 0.5 + 0/%s + 0*%s/(%s+20); // W -> h
qhd = quadHoverDesign(Hh);
