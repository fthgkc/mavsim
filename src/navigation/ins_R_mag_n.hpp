R_mag_n[0][0] = (cosDec2*cosDip2*sigDec2+cosDec2*cosDip2)*sigDip2+cosDec2*cosDip2*sigDec2;
R_mag_n[0][1] = (cosDec*sinDec*cosDip2-cosDec*sinDec*cosDip2*sigDec2)*sigDip2-cosDec*sinDec*cosDip2*sigDec2;
R_mag_n[0][2] = -cosDec*cosDip*sinDip*sigDip2;
R_mag_n[1][0] = (cosDec*sinDec*cosDip2-cosDec*sinDec*cosDip2*sigDec2)*sigDip2-cosDec*sinDec*cosDip2*sigDec2;
R_mag_n[1][1] = (cosDec2*cosDip2*sigDec2+cosDec2*cosDip2)*sigDip2+cosDec2*cosDip2*sigDec2;
R_mag_n[1][2] = -sinDec*cosDip*sinDip*sigDip2;
R_mag_n[2][0] = -cosDec*cosDip*sinDip*sigDip2;
R_mag_n[2][1] = -sinDec*cosDip*sinDip*sigDip2;
R_mag_n[2][2] = cosDip2*sigDip2;