F_att[0+rows_F_att*0] = -a*d*wz-a*c*wy-a*b*wx;
F_att[0+rows_F_att*1] = ((Omega*cosL-2*b*d*wz-2*b*c*wy+(-dd-cc-3*bb-aa)*wx)*R+Ve)/R/2.0;
F_att[0+rows_F_att*2] = -((2*c*d*wz+(dd+3*cc+bb+aa)*wy+2*b*c*wx)*R+Vn)/R/2.0;
F_att[0+rows_F_att*3] = -((Omega*sinL+(3*dd+cc+bb+aa)*wz+2*c*d*wy+2*b*d*wx)*R+Ve*tanL)/R/2.0;
F_att[1+rows_F_att*0] = -((Omega*cosL+(-4*b*d-6*a*c)*wz+(6*a*d-4*b*c)*wy+(3*dd+3*cc-bb-3*aa)*wx)*R+Ve)/R/2.0;
F_att[1+rows_F_att*1] = (2*a*d-b*c)*wz+(b*d+2*a*c)*wy+a*b*wx;
F_att[1+rows_F_att*2] = ((Omega*sinL+(-dd-3*cc-bb+3*aa)*wz+(2*c*d+4*a*b)*wy-6*a*c*wx)*R+Ve*tanL)/R/2.0;
F_att[1+rows_F_att*3] = -(((2*c*d-4*a*b)*wz+(-3*dd-cc-bb+3*aa)*wy+6*a*d*wx)*R+Vn)/R/2.0;
F_att[2+rows_F_att*0] = (((4*c*d-6*a*b)*wz+(-3*dd+cc-3*bb+3*aa)*wy+(6*a*d+4*b*c)*wx)*R+Vn)/R/2.0;
F_att[2+rows_F_att*1] = -((Omega*sinL+(-dd-cc-3*bb+3*aa)*wz+6*a*b*wy+(2*b*d-4*a*c)*wx)*R+Ve*tanL)/R/2.0;
F_att[2+rows_F_att*2] = (2*a*d+b*c)*wz+a*c*wy+(2*a*b-c*d)*wx;
F_att[2+rows_F_att*3] = -((Omega*cosL+(-2*b*d-4*a*c)*wz+6*a*d*wy+(3*dd+cc+bb-3*aa)*wx)*R+Ve)/R/2.0;
F_att[3+rows_F_att*0] = ((Omega*sinL+(dd-3*cc-3*bb+3*aa)*wz+(4*c*d+6*a*b)*wy+(4*b*d-6*a*c)*wx)*R+Ve*tanL)/R/2.0;
F_att[3+rows_F_att*1] = -((6*a*b*wz+(dd+cc+3*bb-3*aa)*wy+(-4*a*d-2*b*c)*wx)*R-Vn)/R/2.0;
F_att[3+rows_F_att*2] = ((Omega*cosL-6*a*c*wz+(4*a*d-2*b*c)*wy+(dd+3*cc+bb-3*aa)*wx)*R+Ve)/R/2.0;
F_att[3+rows_F_att*3] = a*d*wz+(2*a*c-b*d)*wy+(c*d+2*a*b)*wx;
