F(1,1) = a*d*wz+a*c*wy+a*b*wx
F(1,2) = -((Omega*cos(L)-2*b*d*wz-2*b*c*wy+(-d**2-c**2-3*b**2-a**&
2)*wx)*R+Ve)/R/2.0
F(1,3) = ((2*c*d*wz+(d**2-3*c**2+b**2+a**2)*wy+2*b*c*wx)*R+Vn)/R/&
2.0
F(1,4) = ((Omega*sin(L)+(3*d**2+c**2+b**2+a**2)*wz+2*c*d*wy+2*b*d&
*wx)*R+Ve*tan(L))/R/2.0
F(1,5) = c/R/2.0
F(1,6) = (d*tan(L)-b)/R/2.0
F(1,7) = 0
F(1,8) = ((b*Omega*sin(L)+d*Omega*cos(L))*R+d*Ve*sec(L)**2)/R/2.0&

F(1,9) = 0
F(1,10) = -(d*Ve*tan(L)+c*Vn-b*Ve)/R**2/2.0
F(2,1) = ((Omega*cos(L)+(-4*b*d-6*a*c)*wz+(6*a*d-4*b*c)*wy+(3*d**&
2+3*c**2-b**2-3*a**2)*wx)*R+Ve)/R/2.0
F(2,2) = (b*c-2*a*d)*wz+(-b*d-2*a*c)*wy-a*b*wx
F(2,3) = -((Omega*sin(L)+(-d**2-3*c**2-b**2+3*a**2)*wz+(6*c*d+4*a&
*b)*wy-6*a*c*wx)*R+Ve*tan(L))/R/2.0
F(2,4) = (((2*c*d-4*a*b)*wz+(-3*d**2-3*c**2-b**2+3*a**2)*wy+6*a*d&
*wx)*R+Vn)/R/2.0
F(2,5) = d/R/2.0
F(2,6) = -(c*tan(L)-a)/R/2.0
F(2,7) = 0
F(2,8) = -((a*Omega*sin(L)+c*Omega*cos(L))*R+c*Ve*sec(L)**2)/R/2.&
0
F(2,9) = 0
F(2,10) = (c*Ve*tan(L)-d*Vn-a*Ve)/R**2/2.0
F(3,1) = -(((4*c*d-6*a*b)*wz+(-3*d**2-c**2-3*b**2+3*a**2)*wy+(6*a&
*d+4*b*c)*wx)*R+Vn)/R/2.0
F(3,2) = ((Omega*sin(L)+(-d**2-c**2-3*b**2+3*a**2)*wz+6*a*b*wy+(2&
*b*d-4*a*c)*wx)*R+Ve*tan(L))/R/2.0
F(3,3) = (-2*a*d-b*c)*wz+a*c*wy+(c*d-2*a*b)*wx
F(3,4) = ((Omega*cos(L)+(-2*b*d-4*a*c)*wz+6*a*d*wy+(3*d**2+c**2+b&
**2-3*a**2)*wx)*R+Ve)/R/2.0
F(3,5) = -a/R/2.0
F(3,6) = (b*tan(L)+d)/R/2.0
F(3,7) = 0
F(3,8) = -((d*Omega*sin(L)-b*Omega*cos(L))*R-b*Ve*sec(L)**2)/R/2.&
0
F(3,9) = 0
F(3,10) = -(b*Ve*tan(L)-a*Vn+d*Ve)/R**2/2.0
F(4,1) = -((Omega*sin(L)+(d**2-3*c**2-3*b**2+3*a**2)*wz+(4*c*d+6*&
a*b)*wy+(4*b*d-6*a*c)*wx)*R+Ve*tan(L))/R/2.0
F(4,2) = ((6*a*b*wz+(d**2+3*c**2+3*b**2-3*a**2)*wy+(-4*a*d-2*b*c)&
*wx)*R-Vn)/R/2.0
F(4,3) = -((Omega*cos(L)-6*a*c*wz+(4*a*d-6*b*c)*wy+(d**2+3*c**2+b&
**2-3*a**2)*wx)*R+Ve)/R/2.0
F(4,4) = -a*d*wz+(b*d-2*a*c)*wy+(-c*d-2*a*b)*wx
F(4,5) = -b/R/2.0
F(4,6) = -(a*tan(L)+c)/R/2.0
F(4,7) = 0
F(4,8) = ((c*Omega*sin(L)-a*Omega*cos(L))*R-a*Ve*sec(L)**2)/R/2.0&

F(4,9) = 0
F(4,10) = (a*Ve*tan(L)+b*Vn+c*Ve)/R**2/2.0
F(5,1) = 2*c*fz-2*d*fy+2*a*fx
F(5,2) = 2*d*fz+2*c*fy+2*b*fx
F(5,3) = 2*a*fz+2*b*fy-2*c*fx
F(5,4) = 2*b*fz-2*a*fy-2*d*fx
F(5,5) = 0
F(5,6) = (2*Omega*sin(L)*R+Ve*tan(L))/R
F(5,7) = -Vn/R
F(5,8) = 0
F(5,9) = 0
F(5,10) = 0
F(6,1) = -2*b*fz+2*a*fy+2*d*fx
F(6,2) = -2*a*fz-2*b*fy+2*c*fx
F(6,3) = 2*d*fz-2*c*fy+2*b*fx
F(6,4) = 2*c*fz-2*d*fy+2*a*fx
F(6,5) = -(2*Omega*sin(L)*R+Ve*tan(L))/R
F(6,6) = 0
F(6,7) = -(2*Omega*cos(L)*R+Ve)/R
F(6,8) = 0
F(6,9) = 0
F(6,10) = 0
F(7,1) = 2*a*fz+2*b*fy-2*c*fx
F(7,2) = -2*b*fz+2*a*fy+2*d*fx
F(7,3) = -2*c*fz+2*d*fy-2*a*fx
F(7,4) = 2*d*fz+2*c*fy+2*b*fx
F(7,5) = Vn/R
F(7,6) = (2*Omega*cos(L)*R+Ve)/R
F(7,7) = 0
F(7,8) = 0
F(7,9) = 0
F(7,10) = 0
F(8,1) = 0
F(8,2) = 0
F(8,3) = 0
F(8,4) = 0
F(8,5) = 1/R
F(8,6) = 0
F(8,7) = 0
F(8,8) = 0
F(8,9) = 0
F(8,10) = -Vn/R**2
F(9,1) = 0
F(9,2) = 0
F(9,3) = 0
F(9,4) = 0
F(9,5) = 0
F(9,6) = 1/(cos(L)*R)
F(9,7) = 0
F(9,8) = Ve*sin(L)/(cos(L)**2*R)
F(9,9) = 0
F(9,10) = -Ve/(cos(L)*R**2)
F(10,1) = 0
F(10,2) = 0
F(10,3) = 0
F(10,4) = 0
F(10,5) = 0
F(10,6) = 0
F(10,7) = -1
F(10,8) = 0
F(10,9) = 0
F(10,10) = 0
