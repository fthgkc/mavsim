F_vp(0,1) = -(2*Omega*sinL*R+Ve*tanL)/R;
F_vp(0,2) = Vn/R;
F_vp(1,0) = (2*Omega*sinL*R+Ve*tanL)/R;
F_vp(1,2) = (2*Omega*cosL*R+Ve)/R;
F_vp(2,0) = -Vn/R;
F_vp(2,1) = -(2*Omega*cosL*R+Ve)/R;
F_vp(3,0) = 1/R;
F_vp(3,5) = -Vn/RR;
F_vp(4,1) = 1/(cosL*R);
F_vp(4,3) = Ve*sinL/(cosLcosL*R);
F_vp(4,5) = -Ve/(cosL*RR);
F_vp(5,2) = -1;
