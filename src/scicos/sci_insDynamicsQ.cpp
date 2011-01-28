/*sci_insDynamicsQ.cpp
 * Copyright (C) James Goppert 2011 
 * 
 * This file is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Input: 
 *  u1: (imu) fx, fy, fz, wx, wy, wz 
 *  u2: (gravity model) g
 *  u3: (state) a, b, c, d, Vn, Ve, Ve, L, l, h
 * Output:
 *  y1: (state derivative)
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

void sci_insDynamicsQ(scicos_block *block, scicos::enumScicosFlags flag)
{

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * u2=(double*)GetInPortPtrs(block,2);
    double * u3=(double*)GetInPortPtrs(block,3);
    double * y1=(double*)GetOutPortPtrs(block,1);
    double * rpar=block->rpar;

    // aliases
    //
    double & fx     = u1[0];
    double & fy     = u1[1];
    double & fz     = u1[2];
    double & wx     = u1[3];
    double & wy     = u1[4];
    double & wz     = u1[5];
    double & g      = u2[0];
    double & Omega = rpar[0];
    double & Re = rpar[1];
    // Note that l = lon, and not in the equations but left here
    // for ease of use with full state vector x
    double & a      = u3[0];
    double & b      = u3[1];
    double & c      = u3[2];
    double & d      = u3[3];
    double & Vn     = u3[4];
    double & Ve     = u3[5];
    double & Vd     = u3[6];
    double & L      = u3[7];
    double & l      = u3[8];
    double & alt    = u3[9];
         
    //handle flags
    if (flag==scicos::computeOutput)
    {
        const double cosL = cos(L);
        const double sinL = sin(L);
        const double tanL = sinL/cosL;
        const double R = Re+alt;
        const double aa=a*a, ab=a*b, ac=a*c, ad=a*d;
        const double bb=b*b, bc=b*c, bd=b*d;
        const double cc=c*c, cd=c*d;
        const double dd=d*d;

        // This was all hand converted from ins_dynamics_f.f90 that was generated
        // via maxima in the theory folder.
        y1[0] = 0.5*(-d*(Ve*tanL/R+Omega*sinL+(dd-cc-bb+aa)*wz
            +2*(cd+ab)*wy+2*(bd-ac)*wx)-c*(Vn/R+2*(cd-ab)*wz
            +(-dd-cc-bb+aa)*wy+2*(ad+bc)*wx)-b*(-Ve/R-Omega*cosL+2*(bd+ac)*wz
            +2*(bc-ad)*wy+(-dd-cc+bb+aa)*wx));
        y1[1] = 0.5*(c*(Ve*tanL/R+Omega*sinL+(dd-cc-bb+aa)*wz
            +2*(cd+ab)*wy+2*(bd-ac)*wx)-d*(Vn/R+2*(cd-ab)*wz+(-dd-cc-bb+aa)*wy
            +2*(ad+bc)*wx)+a*(-Ve/R-Omega*cosL+2*(bd+ac)*wz+2*(bc-ad)*wy+(-dd-cc+bb+aa)*wx));
        y1[2] = 0.5*(-b*(Ve*tanL/R+Omega*sinL+(dd-cc-bb+aa)*wz+2*(cd+ab)*wy
            +2*(bd-ac)*wx)+a*(Vn/R+2*(cd-ab)*wz+(-dd-cc-bb+aa)*wy
            +2*(ad+bc)*wx)+d*(-Ve/R-Omega*cosL+2*(bd+ac)*wz+2*(bc-ad)*wy+(-dd-cc+bb+aa)*wx));
        y1[3] = 0.5*(a*(Ve*tanL/R+Omega*sinL+(dd-cc-bb+aa)*wz+2*(cd+ab)*wy
            +2*(bd-ac)*wx)+b*(Vn/R+2*(cd-ab)*wz
            +(-dd-cc-bb+aa)*wy+2*(ad+bc)*wx)-c*(-Ve/R-Omega*cosL+2*(bd+ac)*wz
            +2*(bc-ad)*wy+(-dd-cc+bb+aa)*wx));
        y1[4] = -Ve*(-Ve*tanL/R-2*Omega*sinL)-Vd*Vn/R+2*(bd+ac)*fz+2*(bc-ad)*fy+(-dd-cc+bb+aa)*fx;
        y1[5] = Vn*(-Ve*tanL/R-2*Omega*sinL)-Vd*(Ve/R+2*Omega*cosL)+2*(cd-ab)*fz+(-dd-cc-bb+aa)*fy+2*(ad+bc)*fx;
        y1[6] = Ve*(Ve/R+2*Omega*cosL)+Vn*Vn/R+g+(dd-cc-bb+aa)*fz+2*(cd+ab)*fy+2*(bd-ac)*fx;
        y1[7] = Vn/R;
        y1[8] = Ve/(cosL*R);
        y1[9] = -Vd;
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
