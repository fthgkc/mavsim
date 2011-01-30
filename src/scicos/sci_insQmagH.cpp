/*sci_insQmagH.cpp
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

void sci_insQmagH(scicos_block *block, scicos::enumScicosFlags flag)
{

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * u2=(double*)GetInPortPtrs(block,2);
    double * H=(double*)GetOutPortPtrs(block,1);

    // alias names
    double & Bn = u1[0];
    double & Be = u1[1];
    double & Bd = u1[2];

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
            
    //handle flags
    if (flag==scicos::computeOutput)
    {
        memset((void *)H,0,30*sizeof(double));

        static const int rowsH = 3;

        H[1-1+rowsH*(1-1)] = 2*(Be*d-Bd*c+a*Bn);
        H[1-1+rowsH*(2-1)] = 2*(Bd*d+Be*c+b*Bn);
        H[1-1+rowsH*(3-1)] = -2*(Bn*c-b*Be+a*Bd);
        H[1-1+rowsH*(4-1)] = -2*(Bn*d-a*Be-b*Bd);
        H[2-1+rowsH*(1-1)] = -2*(Bn*d-a*Be-b*Bd);
        H[2-1+rowsH*(2-1)] = 2*(Bn*c-b*Be+a*Bd);
        H[2-1+rowsH*(3-1)] = 2*(Bd*d-Be*c+b*Bn);
        H[2-1+rowsH*(4-1)] = -2*(Be*d-Bd*c+a*Bn);
        H[3-1+rowsH*(1-1)] = 2*(Bn*c-b*Be+a*Bd);
        H[3-1+rowsH*(2-1)] = 2*(Bn*d-a*Be-b*Bd);
        H[3-1+rowsH*(3-1)] = 2*(Be*d-Bd*c+a*Bn);
        H[3-1+rowsH*(4-1)] = 2*(Bd*d+Be*c+b*Bn);
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
