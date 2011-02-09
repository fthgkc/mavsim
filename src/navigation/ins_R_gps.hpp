R_gps(1,1) = sigVel**2;
R_gps(2,2) = sigVel**2;
R_gps(3,3) = sigVel**2;
R_gps(4,4) = sigPos**2/R**2;
R_gps(5,5) = sigPos**2/(cos(L)**2*R**2);
R_gps(6,6) = sigAlt**2;
