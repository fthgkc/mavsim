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
    double * f=(double*)GetOutPortPtrs(block,1);
    double * rpar=block->rpar;

    // aliases
    double & wx     = u1[0];
    double & wy     = u1[1];
    double & wz     = u1[2];

    double & fx     = u1[3];
    double & fy     = u1[4];
    double & fz     = u1[5];

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
        const double aa=a*a, bb=b*b, cc=c*c, dd=d*d;

        #include "navigation/ins_dynamics_f.hpp"
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
