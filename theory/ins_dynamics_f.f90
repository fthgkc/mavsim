f(1,1) = -0.5*(-d*(Ve*tan(L)/R+Omega*sin(L)+(d**2-c**2-b**2+a**2)&
*wz+2*(c*d+a*b)*wy+2*(b*d-a*c)*wx)-c*(Vn/R+2*(c*d-a*b)*wz+(-d**2-&
c**2-b**2+a**2)*wy+2*(a*d+b*c)*wx)-b*(-Ve/R-Omega*cos(L)+2*(b*d+a&
*c)*wz+2*(b*c-a*d)*wy+(-d**2-c**2+b**2+a**2)*wx))
f(2,1) = -0.5*(c*(Ve*tan(L)/R+Omega*sin(L)+(d**2-c**2-b**2+a**2)*&
wz+2*(c*d+a*b)*wy+2*(b*d-a*c)*wx)-d*(Vn/R+2*(c*d-a*b)*wz+(-d**2-c&
**2-b**2+a**2)*wy+2*(a*d+b*c)*wx)+a*(-Ve/R-Omega*cos(L)+2*(b*d+a*&
c)*wz+2*(b*c-a*d)*wy+(-d**2-c**2+b**2+a**2)*wx))
f(3,1) = -0.5*(-b*(Ve*tan(L)/R+Omega*sin(L)+(d**2-c**2-b**2+a**2)&
*wz+2*(c*d+a*b)*wy+2*(b*d-a*c)*wx)+a*(Vn/R+2*(c*d-a*b)*wz+(-d**2-&
c**2-b**2+a**2)*wy+2*(a*d+b*c)*wx)+d*(-Ve/R-Omega*cos(L)+2*(b*d+a&
*c)*wz+2*(b*c-a*d)*wy+(-d**2-c**2+b**2+a**2)*wx))
f(4,1) = -0.5*(a*(Ve*tan(L)/R+Omega*sin(L)+(d**2-c**2-b**2+a**2)*&
wz+2*(c*d+a*b)*wy+2*(b*d-a*c)*wx)+b*(Vn/R+2*(c*d-a*b)*wz+(-d**2-c&
**2-b**2+a**2)*wy+2*(a*d+b*c)*wx)-c*(-Ve/R-Omega*cos(L)+2*(b*d+a*&
c)*wz+2*(b*c-a*d)*wy+(-d**2-c**2+b**2+a**2)*wx))
f(5,1) = -Ve*(-Ve*tan(L)/R-2*Omega*sin(L))-Vd*Vn/R+2*(b*d+a*c)*fz&
+2*(b*c-a*d)*fy+(-d**2-c**2+b**2+a**2)*fx
f(6,1) = Vn*(-Ve*tan(L)/R-2*Omega*sin(L))-Vd*(Ve/R+2*Omega*cos(L)&
)+2*(c*d-a*b)*fz+(-d**2-c**2-b**2+a**2)*fy+2*(a*d+b*c)*fx
f(7,1) = Ve*(Ve/R+2*Omega*cos(L))+Vn**2/R+g+(d**2-c**2-b**2+a**2)&
*fz+2*(c*d+a*b)*fy+2*(b*d-a*c)*fx
f(8,1) = Vn/R
f(9,1) = Ve/(cos(L)*R)
f(10,1) = -Vd
