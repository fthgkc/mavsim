F[0][0] = -a*d*wz-a*c*wy-a*b*wx;
F[0][1] = ((Omega*cosL-2*b*d*wz-2*b*c*wy+(-dd-cc-3*bb-aa)*wx)*R+Ve)/R/2.0;
F[0][2] = -((2*c*d*wz+(dd+3*cc+bb+aa)*wy+2*b*c*wx)*R+Vn)/R/2.0;
F[0][3] = -((Omega*sinL+(3*dd+cc+bb+aa)*wz+2*c*d*wy+2*b*d*wx)*R+Ve*tanL)/R/2.0;
F[0][4] = -c/R/2.0;
F[0][5] = -(d*tanL-b)/R/2.0;
F[0][7] = -((b*Omega*sinL+d*Omega*cosL)*R+d*Ve*secLsecL)/R/2.0;
F[0][9] = (d*Ve*tanL+c*Vn-b*Ve)/RR/2.0;
F[1][0] = -((Omega*cosL+(-4*b*d-6*a*c)*wz+(6*a*d-4*b*c)*wy+(3*dd+3*cc-bb-3*aa)*wx)*R+Ve)/R/2.0;
F[1][1] = (2*a*d-b*c)*wz+(b*d+2*a*c)*wy+a*b*wx;
F[1][2] = ((Omega*sinL+(-dd-3*cc-bb+3*aa)*wz+(2*c*d+4*a*b)*wy-6*a*c*wx)*R+Ve*tanL)/R/2.0;
F[1][3] = -(((2*c*d-4*a*b)*wz+(-3*dd-cc-bb+3*aa)*wy+6*a*d*wx)*R+Vn)/R/2.0;
F[1][4] = -d/R/2.0;
F[1][5] = (c*tanL-a)/R/2.0;
F[1][7] = ((a*Omega*sinL+c*Omega*cosL)*R+c*Ve*secLsecL)/R/2.0;
F[1][9] = -(c*Ve*tanL-d*Vn-a*Ve)/RR/2.0;
F[2][0] = (((4*c*d-6*a*b)*wz+(-3*dd+cc-3*bb+3*aa)*wy+(6*a*d+4*b*c)*wx)*R+Vn)/R/2.0;
F[2][1] = -((Omega*sinL+(-dd-cc-3*bb+3*aa)*wz+6*a*b*wy+(2*b*d-4*a*c)*wx)*R+Ve*tanL)/R/2.0;
F[2][2] = (2*a*d+b*c)*wz+a*c*wy+(2*a*b-c*d)*wx;
F[2][3] = -((Omega*cosL+(-2*b*d-4*a*c)*wz+6*a*d*wy+(3*dd+cc+bb-3*aa)*wx)*R+Ve)/R/2.0;
F[2][4] = a/R/2.0;
F[2][5] = -(b*tanL+d)/R/2.0;
F[2][7] = ((d*Omega*sinL-b*Omega*cosL)*R-b*Ve*secLsecL)/R/2.0;
F[2][9] = (b*Ve*tanL-a*Vn+d*Ve)/RR/2.0;
F[3][0] = ((Omega*sinL+(dd-3*cc-3*bb+3*aa)*wz+(4*c*d+6*a*b)*wy+(4*b*d-6*a*c)*wx)*R+Ve*tanL)/R/2.0;
F[3][1] = -((6*a*b*wz+(dd+cc+3*bb-3*aa)*wy+(-4*a*d-2*b*c)*wx)*R-Vn)/R/2.0;
F[3][2] = ((Omega*cosL-6*a*c*wz+(4*a*d-2*b*c)*wy+(dd+3*cc+bb-3*aa)*wx)*R+Ve)/R/2.0;
F[3][3] = a*d*wz+(2*a*c-b*d)*wy+(c*d+2*a*b)*wx;
F[3][4] = b/R/2.0;
F[3][5] = (a*tanL+c)/R/2.0;
F[3][7] = -((c*Omega*sinL-a*Omega*cosL)*R-a*Ve*secLsecL)/R/2.0;
F[3][9] = -(a*Ve*tanL+b*Vn+c*Ve)/RR/2.0;
F[4][0] = 2*c*fz-2*d*fy+2*a*fx;
F[4][1] = 2*d*fz+2*c*fy+2*b*fx;
F[4][2] = 2*a*fz+2*b*fy-2*c*fx;
F[4][3] = 2*b*fz-2*a*fy-2*d*fx;
F[4][5] = -(2*Omega*sinL*R+Ve*tanL)/R;
F[4][6] = Vn/R;
F[5][0] = -2*b*fz+2*a*fy+2*d*fx;
F[5][1] = -2*a*fz-2*b*fy+2*c*fx;
F[5][2] = 2*d*fz+2*c*fy+2*b*fx;
F[5][3] = 2*c*fz-2*d*fy+2*a*fx;
F[5][4] = (2*Omega*sinL*R+Ve*tanL)/R;
F[5][6] = (2*Omega*cosL*R+Ve)/R;
F[6][0] = 2*a*fz+2*b*fy-2*c*fx;
F[6][1] = -2*b*fz+2*a*fy+2*d*fx;
F[6][2] = -2*c*fz+2*d*fy-2*a*fx;
F[6][3] = 2*d*fz+2*c*fy+2*b*fx;
F[6][4] = -Vn/R;
F[6][5] = -(2*Omega*cosL*R+Ve)/R;
F[7][4] = 1/R;
F[7][9] = -Vn/RR;
F[8][5] = 1/(cosL*R);
F[8][7] = Ve*sinL/(cosLcosL*R);
F[8][9] = -Ve/(cosL*RR);
F[9][6] = -1;
