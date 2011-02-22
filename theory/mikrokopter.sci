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
U=0 // forward velocity in m/s
V=0 // side velocity, in m/s
W=0 // vertical velocity, in m/s

// airframe
m=1.02 //kg
Jy=1// guess , moments of inertia
Jz=1// guess
Jx=1// guess

// motor
KV=760 // rpm/Volts 
batVolt=14.8 //Volts
dm=.25 // guess in metres, motor moment arm
tau_motor=36.95991 // guess, motor pole (rad/s)
C_T=0.1 // guess, motor thrust coefficient
C_Q=0.1 // guess, motor torque coefficient

// aerodynamics
rho=1.225 // kg/m^3
rBlade=0.125 // metres
Cd0=0.42; // guess
K_cd_cl=0.02 //guess
s_frame=.1 //guess in m^2
s_frame_side=.1 // guess in m^2
