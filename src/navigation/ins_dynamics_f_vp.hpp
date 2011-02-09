f_vp(1,1) = -Ve*(Ve*tanL/R+2*Omega*sinL)+Vd*Vn/R+(2*b*d+2*a*c)*fz+(2*b*c-2*a*d)*fy+(-dd-cc+bb+aa)*fx;
f_vp(2,1) = -Vn*(-Ve*tanL/R-2*Omega*sinL)-Vd*(-Ve/R-2*Omega*cosL)+(2*c*d-2*a*b)*fz+(-dd+cc-bb+aa)*fy+(2*a*d+2*b*c)*fx;
f_vp(3,1) = -Ve*(Ve/R+2*Omega*cosL)-Vn*Vn/R+g+(dd-cc-bb+aa)*fz+(2*c*d+2*a*b)*fy+(2*b*d-2*a*c)*fx;
f_vp(4,1) = Vn/R;
f_vp(5,1) = Ve/(cosL*R);
f_vp(6,1) = -Vd;
