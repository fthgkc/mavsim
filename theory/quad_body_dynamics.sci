F_body_quad(1,1) = 0;
F_body_quad(1,2) = -wy;
F_body_quad(1,3) = -cos(theta)*g;
F_body_quad(1,4) = -W;
F_body_quad(1,5) = 0;
F_body_quad(1,6) = wz;
F_body_quad(1,7) = 0;
F_body_quad(1,8) = 0;
F_body_quad(1,9) = 0;
F_body_quad(1,10) = V;
F_body_quad(1,11) = 0;
F_body_quad(1,12) = 0;
F_body_quad(1,13) = 0;
F_body_quad(1,14) = 0;
F_body_quad(2,1) = wy;
F_body_quad(2,2) = 0;
F_body_quad(2,3) = -cos(phi)*sin(theta)*g;
F_body_quad(2,4) = U;
F_body_quad(2,5) = 0;
F_body_quad(2,6) = -wx;
F_body_quad(2,7) = -sin(phi)*cos(theta)*g;
F_body_quad(2,8) = -V;
F_body_quad(2,9) = 0;
F_body_quad(2,10) = 0;
F_body_quad(2,11) = -%pi**3*rho*batVolt**2*KV**2*rBlade**4*C_T/m/900.0;
F_body_quad(2,12) = -%pi**3*rho*batVolt**2*KV**2*rBlade**4*C_T/m/900.0;
F_body_quad(2,13) = -%pi**3*rho*batVolt**2*KV**2*rBlade**4*C_T/m/900.0;
F_body_quad(2,14) = -%pi**3*rho*batVolt**2*KV**2*rBlade**4*C_T/m/900.0;
F_body_quad(3,1) = 0;
F_body_quad(3,2) = 0;
F_body_quad(3,3) = 0;
F_body_quad(3,4) = cos(phi);
F_body_quad(3,5) = 0;
F_body_quad(3,6) = 0;
F_body_quad(3,7) = -cos(phi)*wz-sin(phi)*wy;
F_body_quad(3,8) = 0;
F_body_quad(3,9) = 0;
F_body_quad(3,10) = -sin(phi);
F_body_quad(3,11) = 0;
F_body_quad(3,12) = 0;
F_body_quad(3,13) = 0;
F_body_quad(3,14) = 0;
F_body_quad(4,1) = 0;
F_body_quad(4,2) = 0;
F_body_quad(4,3) = 0;
F_body_quad(4,4) = 0;
F_body_quad(4,5) = 0;
F_body_quad(4,6) = 0;
F_body_quad(4,7) = 0;
F_body_quad(4,8) = (Jz-Jx)*wz/Jy;
F_body_quad(4,9) = 0;
F_body_quad(4,10) = (Jz-Jx)*wx/Jy;
F_body_quad(4,11) = 0;
F_body_quad(4,12) = 0;
F_body_quad(4,13) = %pi**3*dm*rho*batVolt**2*KV**2*rBlade**4*C_T/Jy/900.0;
F_body_quad(4,14) = -%pi**3*dm*rho*batVolt**2*KV**2*rBlade**4*C_T/Jy/900.0;
F_body_quad(5,1) = 0;
F_body_quad(5,2) = 0;
F_body_quad(5,3) = cos(theta)*wx-sin(theta)*wz;
F_body_quad(5,4) = 0;
F_body_quad(5,5) = 0;
F_body_quad(5,6) = 0;
F_body_quad(5,7) = 0;
F_body_quad(5,8) = sin(theta);
F_body_quad(5,9) = 0;
F_body_quad(5,10) = cos(theta);
F_body_quad(5,11) = 0;
F_body_quad(5,12) = 0;
F_body_quad(5,13) = 0;
F_body_quad(5,14) = 0;
F_body_quad(6,1) = -wz;
F_body_quad(6,2) = wx;
F_body_quad(6,3) = -sin(phi)*sin(theta)*g;
F_body_quad(6,4) = 0;
F_body_quad(6,5) = 0;
F_body_quad(6,6) = 0;
F_body_quad(6,7) = cos(phi)*cos(theta)*g;
F_body_quad(6,8) = W;
F_body_quad(6,9) = 0;
F_body_quad(6,10) = -U;
F_body_quad(6,11) = 0;
F_body_quad(6,12) = 0;
F_body_quad(6,13) = 0;
F_body_quad(6,14) = 0;
F_body_quad(7,1) = 0;
F_body_quad(7,2) = 0;
F_body_quad(7,3) = (cos(phi)*wz+sin(phi)*wy)/cos(theta)**2;
F_body_quad(7,4) = sin(phi)*sin(theta)/cos(theta);
F_body_quad(7,5) = 0;
F_body_quad(7,6) = 0;
F_body_quad(7,7) = -(sin(phi)*sin(theta)*wz-cos(phi)*sin(theta)*wy)/cos(theta);
F_body_quad(7,8) = 1;
F_body_quad(7,9) = 0;
F_body_quad(7,10) = cos(phi)*sin(theta)/cos(theta);
F_body_quad(7,11) = 0;
F_body_quad(7,12) = 0;
F_body_quad(7,13) = 0;
F_body_quad(7,14) = 0;
F_body_quad(8,1) = 0;
F_body_quad(8,2) = 0;
F_body_quad(8,3) = 0;
F_body_quad(8,4) = -(Jz-Jy)*wz/Jx;
F_body_quad(8,5) = 0;
F_body_quad(8,6) = 0;
F_body_quad(8,7) = 0;
F_body_quad(8,8) = 0;
F_body_quad(8,9) = 0;
F_body_quad(8,10) = -(Jz-Jy)*wy/Jx;
F_body_quad(8,11) = %pi**3*dm*rho*batVolt**2*KV**2*rBlade**4*C_T/Jx/900.0;
F_body_quad(8,12) = -%pi**3*dm*rho*batVolt**2*KV**2*rBlade**4*C_T/Jx/900.0;
F_body_quad(8,13) = 0;
F_body_quad(8,14) = 0;
F_body_quad(9,1) = 0;
F_body_quad(9,2) = 0;
F_body_quad(9,3) = (cos(phi)*sin(theta)*wz+sin(phi)*sin(theta)*wy)/cos(theta)**2;
F_body_quad(9,4) = sin(phi)/cos(theta);
F_body_quad(9,5) = 0;
F_body_quad(9,6) = 0;
F_body_quad(9,7) = -(sin(phi)*wz-cos(phi)*wy)/cos(theta);
F_body_quad(9,8) = 0;
F_body_quad(9,9) = 0;
F_body_quad(9,10) = cos(phi)/cos(theta);
F_body_quad(9,11) = 0;
F_body_quad(9,12) = 0;
F_body_quad(9,13) = 0;
F_body_quad(9,14) = 0;
F_body_quad(10,1) = 0;
F_body_quad(10,2) = 0;
F_body_quad(10,3) = 0;
F_body_quad(10,4) = -(Jy-Jx)*wx/Jz;
F_body_quad(10,5) = 0;
F_body_quad(10,6) = 0;
F_body_quad(10,7) = 0;
F_body_quad(10,8) = -(Jy-Jx)*wy/Jz;
F_body_quad(10,9) = 0;
F_body_quad(10,10) = 0;
F_body_quad(10,11) = %pi**3*rho*batVolt**2*KV**2*C_Q*rBlade**5/Jz/900.0;
F_body_quad(10,12) = %pi**3*rho*batVolt**2*KV**2*C_Q*rBlade**5/Jz/900.0;
F_body_quad(10,13) = -%pi**3*rho*batVolt**2*KV**2*C_Q*rBlade**5/Jz/900.0;
F_body_quad(10,14) = -%pi**3*rho*batVolt**2*KV**2*C_Q*rBlade**5/Jz/900.0;
F_body_quad(11,1) = 0;
F_body_quad(11,2) = 0;
F_body_quad(11,3) = 0;
F_body_quad(11,4) = 0;
F_body_quad(11,5) = 0;
F_body_quad(11,6) = 0;
F_body_quad(11,7) = 0;
F_body_quad(11,8) = 0;
F_body_quad(11,9) = 0;
F_body_quad(11,10) = 0;
F_body_quad(11,11) = -tau_motor;
F_body_quad(11,12) = 0;
F_body_quad(11,13) = 0;
F_body_quad(11,14) = 0;
F_body_quad(12,1) = 0;
F_body_quad(12,2) = 0;
F_body_quad(12,3) = 0;
F_body_quad(12,4) = 0;
F_body_quad(12,5) = 0;
F_body_quad(12,6) = 0;
F_body_quad(12,7) = 0;
F_body_quad(12,8) = 0;
F_body_quad(12,9) = 0;
F_body_quad(12,10) = 0;
F_body_quad(12,11) = 0;
F_body_quad(12,12) = -tau_motor;
F_body_quad(12,13) = 0;
F_body_quad(12,14) = 0;
F_body_quad(13,1) = 0;
F_body_quad(13,2) = 0;
F_body_quad(13,3) = 0;
F_body_quad(13,4) = 0;
F_body_quad(13,5) = 0;
F_body_quad(13,6) = 0;
F_body_quad(13,7) = 0;
F_body_quad(13,8) = 0;
F_body_quad(13,9) = 0;
F_body_quad(13,10) = 0;
F_body_quad(13,11) = 0;
F_body_quad(13,12) = 0;
F_body_quad(13,13) = -tau_motor;
F_body_quad(13,14) = 0;
F_body_quad(14,1) = 0;
F_body_quad(14,2) = 0;
F_body_quad(14,3) = 0;
F_body_quad(14,4) = 0;
F_body_quad(14,5) = 0;
F_body_quad(14,6) = 0;
F_body_quad(14,7) = 0;
F_body_quad(14,8) = 0;
F_body_quad(14,9) = 0;
F_body_quad(14,10) = 0;
F_body_quad(14,11) = 0;
F_body_quad(14,12) = 0;
F_body_quad(14,13) = 0;
F_body_quad(14,14) = -tau_motor;
G_body_quad(1,1) = 0;
G_body_quad(1,2) = 0;
G_body_quad(1,3) = 0;
G_body_quad(1,4) = 0;
G_body_quad(2,1) = 0;
G_body_quad(2,2) = 0;
G_body_quad(2,3) = 0;
G_body_quad(2,4) = 0;
G_body_quad(3,1) = 0;
G_body_quad(3,2) = 0;
G_body_quad(3,3) = 0;
G_body_quad(3,4) = 0;
G_body_quad(4,1) = 0;
G_body_quad(4,2) = 0;
G_body_quad(4,3) = 0;
G_body_quad(4,4) = 0;
G_body_quad(5,1) = 0;
G_body_quad(5,2) = 0;
G_body_quad(5,3) = 0;
G_body_quad(5,4) = 0;
G_body_quad(6,1) = 0;
G_body_quad(6,2) = 0;
G_body_quad(6,3) = 0;
G_body_quad(6,4) = 0;
G_body_quad(7,1) = 0;
G_body_quad(7,2) = 0;
G_body_quad(7,3) = 0;
G_body_quad(7,4) = 0;
G_body_quad(8,1) = 0;
G_body_quad(8,2) = 0;
G_body_quad(8,3) = 0;
G_body_quad(8,4) = 0;
G_body_quad(9,1) = 0;
G_body_quad(9,2) = 0;
G_body_quad(9,3) = 0;
G_body_quad(9,4) = 0;
G_body_quad(10,1) = 0;
G_body_quad(10,2) = 0;
G_body_quad(10,3) = 0;
G_body_quad(10,4) = 0;
G_body_quad(11,1) = tau_motor/4.0;
G_body_quad(11,2) = 0;
G_body_quad(11,3) = tau_motor/2.0;
G_body_quad(11,4) = tau_motor/4.0;
G_body_quad(12,1) = tau_motor/4.0;
G_body_quad(12,2) = 0;
G_body_quad(12,3) = -tau_motor/2.0;
G_body_quad(12,4) = tau_motor/4.0;
G_body_quad(13,1) = tau_motor/4.0;
G_body_quad(13,2) = tau_motor/2.0;
G_body_quad(13,3) = 0;
G_body_quad(13,4) = -tau_motor/4.0;
G_body_quad(14,1) = tau_motor/4.0;
G_body_quad(14,2) = -tau_motor/2.0;
G_body_quad(14,3) = 0;
G_body_quad(14,4) = -tau_motor/4.0;
C_body_quad(1,1) = 1;
C_body_quad(1,2) = 0;
C_body_quad(1,3) = 0;
C_body_quad(1,4) = 0;
C_body_quad(1,5) = 0;
C_body_quad(1,6) = 0;
C_body_quad(1,7) = 0;
C_body_quad(1,8) = 0;
C_body_quad(1,9) = 0;
C_body_quad(1,10) = 0;
C_body_quad(1,11) = 0;
C_body_quad(1,12) = 0;
C_body_quad(1,13) = 0;
C_body_quad(1,14) = 0;
C_body_quad(2,1) = 0;
C_body_quad(2,2) = 1;
C_body_quad(2,3) = 0;
C_body_quad(2,4) = 0;
C_body_quad(2,5) = 0;
C_body_quad(2,6) = 0;
C_body_quad(2,7) = 0;
C_body_quad(2,8) = 0;
C_body_quad(2,9) = 0;
C_body_quad(2,10) = 0;
C_body_quad(2,11) = 0;
C_body_quad(2,12) = 0;
C_body_quad(2,13) = 0;
C_body_quad(2,14) = 0;
C_body_quad(3,1) = 0;
C_body_quad(3,2) = 0;
C_body_quad(3,3) = 1;
C_body_quad(3,4) = 0;
C_body_quad(3,5) = 0;
C_body_quad(3,6) = 0;
C_body_quad(3,7) = 0;
C_body_quad(3,8) = 0;
C_body_quad(3,9) = 0;
C_body_quad(3,10) = 0;
C_body_quad(3,11) = 0;
C_body_quad(3,12) = 0;
C_body_quad(3,13) = 0;
C_body_quad(3,14) = 0;
C_body_quad(4,1) = 0;
C_body_quad(4,2) = 0;
C_body_quad(4,3) = 0;
C_body_quad(4,4) = 1;
C_body_quad(4,5) = 0;
C_body_quad(4,6) = 0;
C_body_quad(4,7) = 0;
C_body_quad(4,8) = 0;
C_body_quad(4,9) = 0;
C_body_quad(4,10) = 0;
C_body_quad(4,11) = 0;
C_body_quad(4,12) = 0;
C_body_quad(4,13) = 0;
C_body_quad(4,14) = 0;
C_body_quad(5,1) = 0;
C_body_quad(5,2) = 0;
C_body_quad(5,3) = 0;
C_body_quad(5,4) = 0;
C_body_quad(5,5) = 1;
C_body_quad(5,6) = 0;
C_body_quad(5,7) = 0;
C_body_quad(5,8) = 0;
C_body_quad(5,9) = 0;
C_body_quad(5,10) = 0;
C_body_quad(5,11) = 0;
C_body_quad(5,12) = 0;
C_body_quad(5,13) = 0;
C_body_quad(5,14) = 0;
C_body_quad(6,1) = 0;
C_body_quad(6,2) = 0;
C_body_quad(6,3) = 0;
C_body_quad(6,4) = 0;
C_body_quad(6,5) = 0;
C_body_quad(6,6) = 1;
C_body_quad(6,7) = 0;
C_body_quad(6,8) = 0;
C_body_quad(6,9) = 0;
C_body_quad(6,10) = 0;
C_body_quad(6,11) = 0;
C_body_quad(6,12) = 0;
C_body_quad(6,13) = 0;
C_body_quad(6,14) = 0;
C_body_quad(7,1) = 0;
C_body_quad(7,2) = 0;
C_body_quad(7,3) = 0;
C_body_quad(7,4) = 0;
C_body_quad(7,5) = 0;
C_body_quad(7,6) = 0;
C_body_quad(7,7) = 1;
C_body_quad(7,8) = 0;
C_body_quad(7,9) = 0;
C_body_quad(7,10) = 0;
C_body_quad(7,11) = 0;
C_body_quad(7,12) = 0;
C_body_quad(7,13) = 0;
C_body_quad(7,14) = 0;
C_body_quad(8,1) = 0;
C_body_quad(8,2) = 0;
C_body_quad(8,3) = 0;
C_body_quad(8,4) = 0;
C_body_quad(8,5) = 0;
C_body_quad(8,6) = 0;
C_body_quad(8,7) = 0;
C_body_quad(8,8) = 1;
C_body_quad(8,9) = 0;
C_body_quad(8,10) = 0;
C_body_quad(8,11) = 0;
C_body_quad(8,12) = 0;
C_body_quad(8,13) = 0;
C_body_quad(8,14) = 0;
C_body_quad(9,1) = 0;
C_body_quad(9,2) = 0;
C_body_quad(9,3) = 0;
C_body_quad(9,4) = 0;
C_body_quad(9,5) = 0;
C_body_quad(9,6) = 0;
C_body_quad(9,7) = 0;
C_body_quad(9,8) = 0;
C_body_quad(9,9) = 1;
C_body_quad(9,10) = 0;
C_body_quad(9,11) = 0;
C_body_quad(9,12) = 0;
C_body_quad(9,13) = 0;
C_body_quad(9,14) = 0;
C_body_quad(10,1) = 0;
C_body_quad(10,2) = 0;
C_body_quad(10,3) = 0;
C_body_quad(10,4) = 0;
C_body_quad(10,5) = 0;
C_body_quad(10,6) = 0;
C_body_quad(10,7) = 0;
C_body_quad(10,8) = 0;
C_body_quad(10,9) = 0;
C_body_quad(10,10) = 1;
C_body_quad(10,11) = 0;
C_body_quad(10,12) = 0;
C_body_quad(10,13) = 0;
C_body_quad(10,14) = 0;
D_body_quad(1,1) = 0;
D_body_quad(1,2) = 0;
D_body_quad(1,3) = 0;
D_body_quad(1,4) = 0;
D_body_quad(2,1) = 0;
D_body_quad(2,2) = 0;
D_body_quad(2,3) = 0;
D_body_quad(2,4) = 0;
D_body_quad(3,1) = 0;
D_body_quad(3,2) = 0;
D_body_quad(3,3) = 0;
D_body_quad(3,4) = 0;
D_body_quad(4,1) = 0;
D_body_quad(4,2) = 0;
D_body_quad(4,3) = 0;
D_body_quad(4,4) = 0;
D_body_quad(5,1) = 0;
D_body_quad(5,2) = 0;
D_body_quad(5,3) = 0;
D_body_quad(5,4) = 0;
D_body_quad(6,1) = 0;
D_body_quad(6,2) = 0;
D_body_quad(6,3) = 0;
D_body_quad(6,4) = 0;
D_body_quad(7,1) = 0;
D_body_quad(7,2) = 0;
D_body_quad(7,3) = 0;
D_body_quad(7,4) = 0;
D_body_quad(8,1) = 0;
D_body_quad(8,2) = 0;
D_body_quad(8,3) = 0;
D_body_quad(8,4) = 0;
D_body_quad(9,1) = 0;
D_body_quad(9,2) = 0;
D_body_quad(9,3) = 0;
D_body_quad(9,4) = 0;
D_body_quad(10,1) = 0;
D_body_quad(10,2) = 0;
D_body_quad(10,3) = 0;
D_body_quad(10,4) = 0;
