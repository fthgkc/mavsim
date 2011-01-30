/*sci_quat2EulerDcm.cpp
 * Copyright (C) Alan Kim, James Goppert 2011 
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
 * u1: a, b, c, d (quaternion)
 * u2: wx, wy, wz (body angular rates)
 *
 * y1: C_nb
 * y1: euler
 * y2: eulerRates
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

void sci_quat2EulerDcm(scicos_block *block, scicos::enumScicosFlags flag)
{

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * u2=(double*)GetInPortPtrs(block,2);
    double * y1=(double*)GetOutPortPtrs(block,1);
    double * y2=(double*)GetOutPortPtrs(block,2);
    double * y3=(double*)GetOutPortPtrs(block,3);

    // alias names
    double & a = u1[0];
    double & b = u1[1];
    double & c = u1[2];
    double & d = u1[3];

    double & wx = u2[0];
    double & wy = u2[1];
    double & wz = u2[2];

    // note: column major in scicoslab
    double & c11 = y1[0];
    double & c21 = y1[1];
    double & c31 = y1[2];
    double & c12 = y1[3];
    double & c22 = y1[4];
    double & c32 = y1[5];
    double & c13 = y1[6];
    double & c23 = y1[7];
    double & c33 = y1[8];

    double & phi = y2[0];
    double & theta = y2[1];
    double & psi = y2[2];

    double & phiRate = y3[0];
    double & thetaRate = y3[1];
    double & psiRate = y3[2];
            
    //handle flags
    if (flag==scicos::computeOutput)
    {
        const double aa = a*a;
        const double bb = b*b;
        const double cc = c*c;
        const double dd = d*d;

        c11 = aa+bb-cc-dd;
        c21 = 2*(b*c+a*d);
        c31 = 2*(b*d-a*c);
        c12 = 2*(b*c-a*d);
        c22 = aa-bb+cc-dd;
        c23 = 2*(c*d+a*b);
        c13 = 2*(b*d+a*c);
        c23 = 2*(c*d-a*b);
        c33 = aa-bb-cc+dd;

        theta = asin(-c31);

        if ( abs(theta-M_PI) < 1e-5 )
        {
            phi = 0;
            psi = atan((c23 - c12)/(c13 + c22));
        }
        else if ( abs(theta+M_PI) < 1e-5 )
        {
            phi = 0;
            psi = atan((c23 + c12)/(c13 - c22));
        }
        else
        {
            phi = atan(c32/c33);
            psi = atan(c21/c11);
        }

        phiRate = (wy*sin(phi) + wz*cos(phi))*tan(theta) + wx;
        thetaRate = (wy*cos(phi) - wz*sin(phi));
        psiRate = (wy*sin(phi) + wz*cos(phi))/cos(theta);
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
