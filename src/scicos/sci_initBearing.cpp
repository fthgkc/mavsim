/*sci_initBearing.cpp
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

void sci_initBearing(scicos_block *block, scicos::enumScicosFlags flag)
{

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * u2=(double*)GetInPortPtrs(block,2);
    double * y1=(double*)GetOutPortPtrs(block,1);

    // alias names
    double & lat1 = u1[0];
    double & lon1 = u1[1];

    double & lat2 = u2[0];
    double & lon2 = u2[1];

    double & psi = y1[0];
            
    //handle flags
    if (flag==scicos::computeOutput)
    {
        const double dLat = lat1-lat2;
        const double dLon = lon1-lon2;
        const double y = sin(dLon) * cos(lat2);
        const double x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(dLon);
        psi = atan2(y,x);
        if(psi < 0) psi+=2*M_PI;
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
