beta=0 // deg
KV=850 // rpm/Volts 
batVolt=11.1 //Volts
rho=1.225 // kg/m^3
rBlade=0.125 // metres
dm=.3 // guess in metres
Cd0=0.02 // guess
K_cd_cl=0.02 //guess
s_frame=.1 //guess in m^2
Vt=2 // guess in m/s
s_frame_side=.1 // guess in m^2
C_T=0.1 // guess
C_Q=0.1 // guess
m=1 //kg
tau_motor=20 // guess
gamma=0 // flight path angle,for level flight is zero
phi=0 //trim
psi=0 // trim
wy=0 //trim
wz=0 //trim
wx=0 //trim
g=9.81 // m/s^2
Jy=1// guess 
Jz=1// guess
Jx=1// guess
dutycycle_Lc=0.1
dutycycle_L=0.1
dutycycle_Rc=0.1
dutycycle_R=0.1
dutycycle_Bc=0.1
dutycycle_B=0.1
dutycycle_Fc=0.1
dutycycle_F=0.1
T_sumSq=.04 // guess in N

deff('[y]=theta_sol(x)','y=(-4*%pi^2*rho*x^2*sin(x)*K_cd_cl-rho*sin(x)*Cd0-2*%pi*rho*x*cos(x))*s_frame*Vt^2+(2*cos(x)*sin(x)^2+2*cos(x)^3)*g*m');
theta=fsolve(-10*%pi/180,theta_sol)
alpha=theta;

T_sumSq_trim=-((1800*%pi^2*rho*theta^2*K_cd_cl+450*rho*Cd0)*s_frame*Vt^2)/(%pi^3*cos(theta)*rho*batVolt^2*KV^2*rBlade^4*C_T)

exec quad_wind_dynamics.sci
