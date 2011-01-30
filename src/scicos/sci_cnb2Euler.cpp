/*sci_cnb2Euler.cpp
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

void sci_cnb2Euler(scicos_block *block, scicos::enumScicosFlags flag)
{

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * Cnb=(double*)GetOutPortPtrs(block,1);

    // alias names
    double & a = u1[0];
    double & b = u1[1];
    double & c = u1[2];
    double & d = u1[3];
            
    //handle flags
    if (flag==scicos::computeOutput)
    {
        memset((void *)Cnb,0,9*sizeof(double));
        
        static const int rowsH = 3;
        const double aa = a*a;
        const double bb = b*b;
        const double cc = c*c;
        const double dd = d*d;

        Cnb[0] = aa+bb-cc-dd;
        Cnb[1]=2*(b*c+a*d);
        Cnb[2]=2*(b*d-a*c);
        Cnb[3]=2*(b*c-a*d);
        Cnb[4]=aa-bb+cc-dd;
        Cnb[5]=2*(c*d+a*b);
        Cnb[6]=2*(b*d+a*c);
        Cnb[7]=2*(c*d-a*b);
        Cnb[8]=aa-bb-cc+dd;
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
