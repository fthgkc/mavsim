/*sci_insQmagH.cpp
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
 * u1: dip (inclination), dec (declination) (rad)
 * u2: sig dip, sig dec (rad)
 * u3: x (state)
 *
 * Out1 = H_mag (3x10)
 * Out2 = R_mag_n (3x3) : note this is in the navigation frame,
 *   you can use C_nb for a similarity transformation
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

void sci_insQmagH(scicos_block *block, scicos::enumScicosFlags flag)
{

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * u2=(double*)GetInPortPtrs(block,2);
    double * u3=(double*)GetInPortPtrs(block,3);
    double * H_mag=(double*)GetOutPortPtrs(block,1);
    double * R_mag_n=(double*)GetOutPortPtrs(block,2);

    // alias names
    double & dip = u1[0];
    double & dec = u1[1];
    double & sigDip = u2[0];
    double & sigDec = u2[1];

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
        memset((void *)H_mag,0,30*sizeof(double));
        memset((void *)R_mag_n,0,9*sizeof(double));
        double sigDec2 = sigDec*sigDec;
        double sigDip2 = sigDip*sigDip;
        double cosDec = cos(dec), sinDec = sin(dec);
        double cosDec2 = cosDec*cosDec, sinDec2 = sinDec*sinDec;
        double cosDip = cos(dip), sinDip = sin(dip);
        double cosDip2 = cosDip*cosDip, sinDip2 = sinDip*sinDip;
        double Bn = cosDec*cosDip;
        double Be = sinDec*cosDip;
        double Bd = sinDip;

        static const int rows_H_mag = 3;
        #include "navigation/ins_dynamics_H_mag.hpp" 
        static const int rows_R_mag_n = 3;
        #include "navigation/ins_dynamics_R_mag_n.hpp" 
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
