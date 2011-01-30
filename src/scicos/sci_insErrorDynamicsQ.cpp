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
       
        const double cosL = cos(L);
        const double sinL = sin(L);
        const double tanL = cosL/sinL;
        const double cosLcosL = cosL*cosL;     
        const double secLsecL = 1/cosLcosL;
        const double R = Re+alt;
        const double aa = a*a;
        const double bb = b*b;
        const double cc = c*c;
        const double dd = d*d;
        const double aaa = aa*a;
        const double bbb = bb*b;
        const double ccc = cc*c;
        const double ddd = dd*d;
        const double RR = R*R;


        static const int rowsF = 10 ;
        F[1-1+rowsF*(1-1)] = a*d*wz+a*c*wy+a*b*wx;
        F[1-1+rowsF*(2-1)] = -((Omega*cosL-2*b*d*wz-2*b*c*wy+(-dd-cc-3*bb-aa)*wx)*R+Ve)/R/2.0;
        F[1-1+rowsF*(3-1)] = ((2*c*d*wz+(dd-3*cc+bb+aa)*wy+2*b*c*wx)*R+Vn)/R/2.0;
        F[1-1+rowsF*(4-1)] = ((Omega*sinL+(3*dd+cc+bb+aa)*wz+2*c*d*wy+2*b*d*wx)*R+Ve*tanL)/R/2.0;
        F[1-1+rowsF*(5-1)] = c/R/2.0;
        F[1-1+rowsF*(6-1)] = (d*tanL-b)/R/2.0;
        F[1-1+rowsF*(8-1)] = ((b*Omega*sinL+d*Omega*cosL)*R+d*Ve*secLsecL)/R/2.0;
        F[1-1+rowsF*(10-1)] = -(d*Ve*tanL+c*Vn-b*Ve)/RR/2.0;
        F[2-1+rowsF*(1-1)] = ((Omega*cosL+(-4*b*d-6*a*c)*wz+(6*a*d-4*b*c)*wy+(3*dd+3*cc-bb-3*aa)*wx)*R+Ve)/R/2.0;
        F[2-1+rowsF*(2-1)] = (b*c-2*a*d)*wz+(-b*d-2*a*c)*wy-a*b*wx;
        F[2-1+rowsF*(3-1)] = -((Omega*sinL+(-dd-3*cc-bb+3*aa)*wz+(6*c*d+4*a*b)*wy-6*a*c*wx)*R+Ve*tanL)/R/2.0;
        F[2-1+rowsF*(4-1)] = (((2*c*d-4*a*b)*wz+(-3*dd-3*cc-bb+3*aa)*wy+6*a*d*wx)*R+Vn)/R/2.0;
        F[2-1+rowsF*(5-1)] = d/R/2.0;
        F[2-1+rowsF*(6-1)] = -(c*tanL-a)/R/2.0;
        F[2-1+rowsF*(7-1)] = 0;
        F[2-1+rowsF*(8-1)] = -((a*Omega*sinL+c*Omega*cosL)*R+c*Ve*secLsecL)/R/2.0;
        F[2-1+rowsF*(10-1)] = (c*Ve*tanL-d*Vn-a*Ve)/RR/2.0;
        F[3-1+rowsF*(1-1)] = -(((4*c*d-6*a*b)*wz+(-3*dd-cc-3*bb+3*aa)*wy+(6*a*d+4*b*c)*wx)*R+Vn)/R/2.0;
        F[3-1+rowsF*(2-1)] = ((Omega*sinL+(-dd-cc-3*bb+3*aa)*wz+6*a*b*wy+(2*b*d-4*a*c)*wx)*R+Ve*tanL)/R/2.0;
        F[3-1+rowsF*(3-1)] = (-2*a*d-b*c)*wz+a*c*wy+(c*d-2*a*b)*wx;
        F[3-1+rowsF*(4-1)] = ((Omega*cosL+(-2*b*d-4*a*c)*wz+6*a*d*wy+(3*dd+cc+bb-3*aa)*wx)*R+Ve)/R/2.0;
        F[3-1+rowsF*(5-1)] = -a/R/2.0;
        F[3-1+rowsF*(6-1)] = (b*tanL+d)/R/2.0;
        F[3-1+rowsF*(8-1)] = -((d*Omega*sinL-b*Omega*cosL)*R-b*Ve*secLsecL)/R/2.0;
        F[3-1+rowsF*(10-1)] = -(b*Ve*tanL-a*Vn+d*Ve)/RR/2.0;
        F[4-1+rowsF*(1-1)] = -((Omega*sinL+(dd-3*cc-3*bb+3*aa)*wz+(4*c*d+6*a*b)*wy+(4*b*d-6*a*c)*wx)*R+Ve*tanL)/R/2.0;
        F[4-1+rowsF*(2-1)] = ((6*a*b*wz+(dd+3*cc+3*bb-3*aa)*wy+(-4*a*d-2*b*c)*wx)*R-Vn)/R/2.0;
        F[4-1+rowsF*(3-1)] = -((Omega*cosL-6*a*c*wz+(4*a*d-6*b*c)*wy+(dd+3*cc+bb-3*aa)*wx)*R+Ve)/R/2.0;
        F[4-1+rowsF*(4-1)] = -a*d*wz+(b*d-2*a*c)*wy+(-c*d-2*a*b)*wx;
        F[4-1+rowsF*(5-1)] = -b/R/2.0;
        F[4-1+rowsF*(6-1)] = -(a*tanL+c)/R/2.0;
        F[4-1+rowsF*(7-1)] = 0;
        F[4-1+rowsF*(8-1)] = ((c*Omega*sinL-a*Omega*cosL)*R-a*Ve*secLsecL)/R/2.0;
        F[4-1+rowsF*(10-1)] = (a*Ve*tanL+b*Vn+c*Ve)/RR/2.0;
        F[5-1+rowsF*(1-1)] = 2*c*fz-2*d*fy+2*a*fx;
        F[5-1+rowsF*(2-1)] = 2*d*fz+2*c*fy+2*b*fx;
        F[5-1+rowsF*(3-1)] = 2*a*fz+2*b*fy-2*c*fx;
        F[5-1+rowsF*(4-1)] = 2*b*fz-2*a*fy-2*d*fx;
        F[5-1+rowsF*(5-1)] = 0;
        F[5-1+rowsF*(6-1)] = (2*Omega*sinL*R+Ve*tanL)/R;
        F[5-1+rowsF*(7-1)] = -Vn/R;
        F[6-1+rowsF*(1-1)] = -2*b*fz+2*a*fy+2*d*fx;
        F[6-1+rowsF*(2-1)] = -2*a*fz-2*b*fy+2*c*fx;
        F[6-1+rowsF*(3-1)] = 2*d*fz-2*c*fy+2*b*fx;
        F[6-1+rowsF*(4-1)] = 2*c*fz-2*d*fy+2*a*fx;
        F[6-1+rowsF*(5-1)] = -(2*Omega*sinL*R+Ve*tanL)/R;
        F[6-1+rowsF*(7-1)] = -(2*Omega*cosL*R+Ve)/R;
        F[7-1+rowsF*(1-1)] = 2*a*fz+2*b*fy-2*c*fx;
        F[7-1+rowsF*(2-1)] = -2*b*fz+2*a*fy+2*d*fx;
        F[7-1+rowsF*(3-1)] = -2*c*fz+2*d*fy-2*a*fx;
        F[7-1+rowsF*(4-1)] = 2*d*fz+2*c*fy+2*b*fx;
        F[7-1+rowsF*(5-1)] = Vn/R;
        F[7-1+rowsF*(6-1)] = (2*Omega*cosL*R+Ve)/R;
        F[8-1+rowsF*(5-1)] = 1/R;
        F[8-1+rowsF*(10-1)] = -Vn/RR;
        F[9-1+rowsF*(6-1)] = 1/(cosL*R);
        F[9-1+rowsF*(8-1)] = Ve*sinL/(cosLcosL*R);
        F[9-1+rowsF*(10-1)] = -Ve/(cosL*RR);
        F[10-1+rowsF*(7-1)] = -1;
                
        static const int rowsG = 10;
        G[1-1+rowsG*(1-1)] = (b*dd+b*cc+bbb+aa*b)/2.0;
        G[1-1+rowsG*(2-1)] = (c*dd-ccc+(bb+aa)*c)/2.0;
        G[1-1+rowsG*(3-1)] = (ddd+(cc+bb+aa)*d)/2.0;
        G[2-1+rowsG*(1-1)] = (3*a*dd+3*a*cc-a*bb-aaa)/2.0;
        G[2-1+rowsG*(2-1)] = -(ddd+(3*cc+bb-3*aa)*d+4*a*b*c)/2.0;
        G[2-1+rowsG*(3-1)] = (c*dd-4*a*b*d+ccc+(bb-3*aa)*c)/2.0;
        G[3-1+rowsG*(1-1)] = (ddd+(cc+bb-3*aa)*d-4*a*b*c)/2.0;
        G[3-1+rowsG*(2-1)] = (3*a*dd+a*cc+3*a*bb-aaa)/2.0;
        G[3-1+rowsG*(3-1)] = -(b*dd+4*a*c*d+b*cc+bbb-3*aa*b)/2.0;
        G[4-1+rowsG*(1-1)] = -(c*dd+4*a*b*d+ccc+(bb-3*aa)*c)/2.0;
        G[4-1+rowsG*(2-1)] = (b*dd-4*a*c*d+3*b*cc+bbb-3*aa*b)/2.0;
        G[4-1+rowsG*(3-1)] = -(a*dd-3*a*cc-3*a*bb+aaa)/2.0;
        G[5-1+rowsG*(4-1)] = -dd-cc+bb+aa;
        G[5-1+rowsG*(5-1)] = 2*b*c-2*a*d;
        G[5-1+rowsG*(6-1)] = 2*b*d+2*a*c;
        G[6-1+rowsG*(4-1)] = 2*a*d+2*b*c;
        G[6-1+rowsG*(5-1)] = -dd-cc-bb+aa;
        G[6-1+rowsG*(6-1)] = 2*c*d-2*a*b;
        G[7-1+rowsG*(4-1)] = 2*b*d-2*a*c;
        G[7-1+rowsG*(5-1)] = 2*c*d+2*a*b;
        G[7-1+rowsG*(6-1)] = dd-cc-bb+aa;

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
