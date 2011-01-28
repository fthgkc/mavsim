/*sci_insErrorDynamicsQ.cpp
 * Copyright (C) James Goppert 2011 
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
 * Input: 
 *  x: a, b, c, d, Vn, Ve, Ve, L, l, h
 * Output:
 *  dx 
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
    double * u=(double*)GetInPortPtrs(block,1);
    double * dx=(double*)GetOutPortPtrs(block,1);
    double * rpar=block->rpar;

    // aliases
    //
    // Note that l = lon, and not in the equations but left here
    // for ease of use with full state vector x
    double & a      = u[0];
    double & b      = u[1];
    double & c      = u[2];
    double & d      = u[3];
    double & Vn     = u[4];
    double & Ve     = u[5];
    double & Vd     = u[6];
    double & L      = u[7];
    double & l      = u[8];
    double & alt    = u[9];
    double & Omega = rpar[0];
    double & Re = rpar[1];

            
    //handle flags
    if (flag==scicos::computeOutput)
    {
        const double cL = cos(L);
        const double sL = sin(L);
        const double tL = sL/cL;
        const double R = Re+alt;
        double aa=a*a, bb=b*b, cc=c*c, dd=d*d;
i
        // This was all basically copy and pasted from maxima, see insErrorDynamics.wx in the
        // theory folder of oooark.
        dx[0] = 0.5*(-d*((Ve*tL)/R+Omega*sL+(dd-cc-bb+aa)*wz+2*(c*d+a*b)*wy+2*(b*d-a*c)*wx)
                -c*(Vn/R+2*(c*d-a*b)*wz+(-dd-cc-bb+aa)*wy+2*(a*d+b*c)*wx)
                -b*(-Ve/R-Omega*cL+2*(b*d+a*c)*wz+2*(b*c-a*d)*wy+(-dd-cc+bb+aa)*wx));
        dx[1] = 0.5*(c*((Ve*tL)/R+Omega*sL+(dd-cc-bb+aa)*wz+2*(c*d+a*b)*wy+2*(b*d-a*c)*wx)
                -d*(Vn/R+2*(c*d-a*b)*wz+(-dd-cc-bb+aa)*wy+2*(a*d+b*c)*wx)+a*(-Ve/R-Omega*cL
                +2*(b*d+a*c)*wz+2*(b*c-a*d)*wy+(-dd-cc+bb+aa)*wx));
        dx[2] = 0.5*(-b*((Ve*tL)/R+Omega*sL+(dd-cc-bb+aa)*wz+2*(c*d+a*b)*wy
                +2*(b*d-a*c)*wx)+a*(Vn/R+2*(c*d-a*b)*wz+(-dd-cc-bb+aa)*wy+2*(a*d+b*c)*wx)
                +d*(-Ve/R-Omega*cL+2*(b*d+a*c)*wz+2*(b*c-a*d)*wy+(-dd-cc+bb+aa)*wx));
        dx[3] = 0.5*(a*((Ve*tL)/R+Omega*sL+(dd-cc-bb+aa)*wz+2*(c*d+a*b)*wy
                +2*(b*d-a*c)*wx)+b*(Vn/R+2*(c*d-a*b)*wz+(-dd-cc-bb+aa)*wy+2*(a*d+b*c)*wx)
                -c*(-Ve/R-Omega*cL+2*(b*d+a*c)*wz+2*(b*c-a*d)*wy+(-dd-cc+bb+aa)*wx));
        dx[4] = -Ve*(-(Ve*tL)/R-2*Omega*sL)-(Vd*Vn)/R+2*(b*d+a*c)*fz+2*(b*c-a*d)*fy+(-dd-cc+bb+aa)*fx;
        dx[5] = Vn*(-(Ve*tL)/R-2*Omega*sL)-Vd*(Ve/R+2*Omega*cos(L))+2*(c*d-a*b)*fz+(-dd-cc-bb+aa)*fy+2*(a*d+b*c)*fx;
        dx[6] = Ve*(Ve/R+2*Omega*cos(L))+Vn*Vn/R+g+(dd-cc-bb+aa)*fz+2*(c*d+a*b)*fy+2*(b*d-a*c)*fx;
        dx[7] = Vn/R;
        dx[8] = Ve/(cL*R);
        dx[9] = -Vd;

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
