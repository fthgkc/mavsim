R_gps[0+rows_R_gps*0] = sigVel**2;
R_gps[1+rows_R_gps*1] = sigVel**2;
R_gps[2+rows_R_gps*2] = sigVel**2;
R_gps[3+rows_R_gps*3] = sigPos**2/R**2;
R_gps[4+rows_R_gps*4] = sigPos**2/(cos(L)**2*R**2);
R_gps[5+rows_R_gps*5] = sigAlt**2;
