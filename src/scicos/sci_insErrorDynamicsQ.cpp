/*sci_insErrorDynamicsQ.cpp
 * Copyright (C) Alan Kim, James Goppert 2011 
 * 
 * sci_insErrorDynamics.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sci_insErrorDynamics.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * u1: fn, fe, fd
 * u2: roll, pitch, yaw, Vn, Ve, Vd, Lat, Lon, alt
 *
 * Out1 = F (9x9)
 * Out2 = G (9x6)
 *
 * for del_x_dot = F*del_x + G*u
 * where u del_x is the error state of INS
 * and u is the inputs to the INS error dynamics
 *
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include "math/GpsIns.hpp"
#include "utilities.hpp"
#include <stdexcept>

extern "C"
{

#include <scicos/scicos_block4.h>
#include <math.h>
#include "definitions.hpp"

void sci_insErrorDynamicsQ(scicos_block *block, scicos::enumScicosFlags flag)
{

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * u2=(double*)GetInPortPtrs(block,2);
    double * F=(double*)GetOutPortPtrs(block,1);
    double * G=(double*)GetOutPortPtrs(block,2);
    double * rpar=block->rpar;

    // alias names
    double & fx = u1[0];
    double & fy = u1[1];
    double & fz = u1[2];
    double & wx = u1[3];
    double & wy = u1[4];
    double & wz = u1[5];

    // Note that l = lon, and not in the equations but left here
    // for ease of use with full state vector x
    double & a      = u2[0];
    double & b      = u2[1];
    double & c      = u2[2];
    double & d      = u2[3];
    double & Vn     = u2[4];
    double & Ve     = u2[5];
    double & Vd     = u2[6];
    double & L      = u2[7];
    double & l      = u2[8];
    double & alt    = u2[9];
    double & Omega = rpar[0];
    double & Re = rpar[1];

            
    //handle flags
    if (flag==scicos::computeOutput)
    {
        memset((void *)F,0,100*sizeof(double));
        memset((void *)G,0,60*sizeof(double));
       
        const double cL = cos(L);
        const double sL = sin(L);
        const double R = Re+alt;

        static const int rowsF = 10 ;

        F[0+rowsF*0]=-a*d*wz-a*c*wy-a*b*wx;
        F[0+rowsF*1]=-((Omega*cL*sL-Omega*cL*cL+((3*d*d+(2*c+2*b)*d+cc+b*b+a*a)*wz+(d*d+2*c*d-3*cc+2*b*c+b*b+a*a)*wy+i
                        (d*d+2*b*d+(cc)+2*b*c+3*(b*b)+(a*a))*wx)*cL)*R+Ve*sL+(Vn-Ve)*cL)/(2*cL*R);
        F[0+rowsF*4]=-c/(2*R);
        F[0+rowsF*5]=-(d*sL-b*cL)/(2*cL*R);
        F[0+rowsF*7]=-((b*Omega*(cL*cL)*sL+d*Omega*(cL*cL*cL))*R+d*Ve)/(2*(cL*cL)*R);
	F[0+rowsF*9]=(d*Ve*sL+(c*Vn-b*Ve)*cL)/(2*cL*(R*R));
        F[1+rowsF*0]=-((Omega*cL+(-4*b*d-6*a*c)*wz+(6*a*d-4*b*c)*wy+(3*(d*d)+3*(cc)-(b*b)-3*(a*a))*wx)*R+Ve)/(2*R);
        F[1+rowsF*1]=((Omega*cL*sL+((-(d*d)+(4*a-2*c)*d-3*(cc)-2*b*c-(b*b)+4*a*b+3*(a*a))*wz+(3*(d*d)+(6*c+2*b)*d+i
                            3*(cc)+4*a*c+(b*b)+4*a*b-3*(a*a))*wy+(-6*a*d-6*a*c+2*a*b)*wx)*cL)*R+Ve*sL-Vn*cL)/(2*cL*R);
        F[1+rowsF*4]=-d/(2*R);
        F[1+rowsF*5]=(c*sL-a*cL)/(2*cL*R);
        F[1+rowsF*7]=((a*Omega*(cL*cL)*sL+c*Omega*(cL*cL*cL))*R+c*Ve)/(2*(cL*cL)*R);
        F[1+rowsF*9]=-(c*Ve*sL+(-d*Vn-a*Ve)*cL)/(2*cL*(R*R));
        F[2+rowsF*0]=(((4*c*d-6*a*b)*wz+(-3*(d*d)-(cc)-3*(b*b)+3*(a*a))*wy+(6*a*d+4*b*c)*wx)*R+Vn)/(2*R);
        F[2+rowsF*1]=-((Omega*cL*sL+Omega*(cL*cL)+((-(d*d)+(-2*b-4*a)*d-(cc)+(-2*b-4*a)*c-3*(b*b)+3*(a*a))*wz+
                        (6*a*d+2*a*c+6*a*b)*wy+(3*(d*d)+(2*c+2*b)*d+(cc)-4*a*c+(b*b)-4*a*b-3*(a*a))*wx)*cL)*R+Ve*sL+Ve*cL)/(2*cL*R);
        F[2+rowsF*4]=a/(2*R);
        F[2+rowsF*5]=-(b*sL+d*cL)/(2*cL*R);
        F[2+rowsF*7]=((d*Omega*(cL*cL)*sL-b*Omega*(cL*cL*cL))*R-b*Ve)/(2*(cL*cL)*R);
        F[2+rowsF*9]=(b*Ve*sL+(d*Ve-a*Vn)*cL)/(2*cL*(R*R));
        F[3+rowsF*0]=((Omega*cL*sL+(((d*d)-3*(cc)-3*(b*b)+3*(a*a))*wz+(4*c*d+6*a*b)*wy+(4*b*d-6*a*c)*wx)*cL)*R+Ve*sL)/(2*cL*R);
        F[3+rowsF*1]=((Omega*cL+(2*a*d-6*a*c-6*a*b)*wz+(-(d*d)+(4*a-2*b)*d-3*(cc)+(4*a-6*b)*c-3*(b*b)+i
                        3*(a*a))*wy+((d*d)+(2*c+4*a)*d+3*(cc)+2*b*c+(b*b)+4*a*b-3*(a*a))*wx)*R+Vn+Ve)/(2*R);
        F[3+rowsF*4]=b/(2*R);
        F[3+rowsF*5]=(a*sL+ccL)/(2*cL*R);
        F[3+rowsF*7]=-((c*Omega*(cL*cL)*sL-a*Omega*(cL*cL*cL))*R-a*Ve)/(2*(cL*cL)*R);
        F[3+rowsF*9]=-(a*Ve*sL+(b*Vn+c*Ve)*cL)/(2*cL*(R*R));
        F[4+rowsF*0]=2*c*fz-2*d*fy+2*a*fx;
        F[4+rowsF*1]=(2*d+2*b+2*a)*fz+(2*c+2*b-2*a)*fy+(-2*d-2*c+2*b)*fx;
        F[4+rowsF*4]=Vd/R;
        F[4+rowsF*5]=-(2*Omega*cL*sL*R+2*Ve*sL)/(cL*R);
        F[4+rowsF*6]=Vn/R;
        F[4+rowsF*7]=-(2*Omega*Ve*(cL*cL*cL)*R+(Ve*Ve))/((cL*cL)*R);
        F[4+rowsF*9]=((Ve*Ve)*sL-Vd*Vn*cL)/(cL*(R*R));
        F[5+rowsF*0]=-2*b*fz+2*a*fy+2*d*fx;
        F[5+rowsF*1]=(2*d+2*c-2*a)*fz+(-2*d-2*c-2*b)*fy+(2*c+2*b+2*a)*fx;
        F[5+rowsF*4]=(2*Omega*cL*sL*R+Ve*sL)/(cL*R);
        F[5+rowsF*5]=(Vn*sL+Vd*cL)/(cL*R);
        F[5+rowsF*6]=(2*Omega*cL*R+Ve)/R;
        F[5+rowsF*7]=-((2*Omega*Vd*(cL*cL)*sL-2*Omega*Vn*(cL*cL*cL))*R-Ve*Vn)/((cL*cL)*R);
        F[5+rowsF*9]=-(Ve*Vn*sL+Vd*Ve*cL)/(cL*(R*R));
        F[6+rowsF*0]=2*a*fz+2*b*fy-2*c*fx;
        F[6+rowsF*1]=(2*d-2*c-2*b)*fz+(2*d+2*c+2*a)*fy+(2*d+2*b-2*a)*fx;
        F[6+rowsF*4]=-(2*Vn)/R;
        F[6+rowsF*5]=-(2*Omega*cL*R+2*Ve)/R;
        F[6+rowsF*7]=2*Omega*Ve*sL;
        F[6+rowsF*9]=((Vn*Vn)+(Ve*Ve))/(R*R);
        F[7+rowsF*4]=1/R;
        F[7+rowsF*9]=-Vn/(R*R);
        F[8+rowsF*5]=1/(R*cL);
        F[8+rowsF*7]=Ve*sL/(R*cL*cL);
        F[8+rowsF*9]=-Ve/(R*R*cL);
        F[9+rowsF*5]=-1;

        static const int rowsG = 10;

        G[0+rowsG*0]=-G[2+rowsG*2];
        G[0+rowsG*1]=-(c*(d*d)*(R*R)-(cc*c)*(R*R)+(b*b)*c*(R*R)+(a*a)*c*(R*R))/(2*(R*R));
        G[0+rowsG*2]=-((d*d*d)*(R*R)+(cc)*d*(R*R)+(b*b)*d*(R*R)+(a*a)*d*(R*R))/(2*(R*R));
        G[1+rowsG*0]=(-3*a*(d*d)*(R*R)-3*a*(cc)*(R*R)+a*(b*b)*(R*R)+(a*a*a)*(R*R))/(2*(R*R));
        G[1+rowsG*1]=((d*d*d)*(R*R)+3*(cc)*d*(R*R)+(b*b)*d*(R*R)-3*(a*a)*d*(R*R)+4*a*b*c*(R*R))/(2*(R*R));
        G[1+rowsG*2]=(-c*(d*d)*(R*R)+4*a*b*d*(R*R)-(cc*c)*(R*R)-(b*b)*c*(R*R)+3*(a*a)*c*(R*R))/(2*(R*R));
        G[2+rowsG*0]=-((d*d*d)*(R*R)+(cc)*d*(R*R)+(b*b)*d*(R*R)-3*(a*a)*d*(R*R)-4*a*b*c*(R*R))/(2*(R*R));
        G[2+rowsG*1]=-(3*a*(d*d)*(R*R)+a*(cc)*(R*R)+3*a*(b*b)*(R*R)-(a*a*a)*(R*R))/(2*(R*R));
        G[2+rowsG*2]=-(-b*(d*d)*(R*R)-4*a*c*d*(R*R)-b*(cc)*(R*R)-(b*b*b)*(R*R)+3*(a*a)*b*(R*R))/(2*(R*R));
        G[3+rowsG*0]=(c*(d*d)*(R*R)+4*a*b*d*(R*R)+(cc*c)*(R*R)+(b*b)*c*(R*R)-3*(a*a)*c*(R*R))/(2*(R*R));
        G[3+rowsG*1]=(-b*(d*d)*(R*R)+4*a*c*d*(R*R)-3*b*(cc)*(R*R)-(b*b*b)*(R*R)+3*(a*a)*b*(R*R))/(2*(R*R));
        G[3+rowsG*2]=(a*(d*d)*(R*R)-3*a*(cc)*(R*R)-3*a*(b*b)*(R*R)+(a*a*a)*(R*R))/(2*(R*R));
        G[4+rowsG*3]=-(d*d)-(cc)+(b*b)+(a*a);
        G[4+rowsG*4]=2*(b*c-a*d);
        G[4+rowsG*5]=2*(b*d+a*c);
        G[5+rowsG*3]=2*(a*d+b*c);
        G[5+rowsG*4]=-(d*d)-(cc)-(b*b)+(a*a);
        G[5+rowsG*5]=2*(c*d-a*b);
        G[6+rowsG*3]=2*(b*d-a*c);
        G[6+rowsG*4]=2*(c*d+a*b);
        G[6+rowsG*5]=(d*d)-(cc)-(b*b)+(a*a);
    }
    else if (flag==scicos::terminate)
    {
    }
    else if (flag==scicos::initialize || flag==scicos::reinitialize)
    {
    }
    else
    {
        std::cout << "unhandled block flag: " << flag << std::endl;
    }
}

} // extern c

// vim:ts=4:sw=4:expandtab
