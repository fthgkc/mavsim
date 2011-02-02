f[0] = -Ve*(Ve*tanL/R+2*Omega*sinL)+Vd*Vn/R+2*(b*d+a*c)*fz+2*(b*c-a*d)*fy+(-dd-cc+bb+aa)*fx;
f[1] = -Vn*(-Ve*tanL/R-2*Omega*sinL)-Vd*(-Ve/R-2*Omega*cosL)+2*(c*d-a*b)*fz+(-dd+cc-bb+aa)*fy+2*(a*d+b*c)*fx;
f[2] = -Ve*(Ve/R+2*Omega*cosL)-Vn*Vn/R+g+(dd-cc-bb+aa)*fz+2*(c*d+a*b)*fy+2*(b*d-a*c)*fx;
f[3] = Vn/R;
f[4] = Ve/(cosL*R);
f[5] = -Vd;
