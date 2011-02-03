/*sci_magMeasModel.cpp
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
#include <cstdio>

extern "C"
{

#include <scicos/scicos_block4.h>
#include <math.h>
#include "definitions.hpp"

void sci_magMeasModel(scicos_block *block, scicos::enumScicosFlags flag)
{
    enum ins_mode {INS_FULL_STATE=0,INS_ATT_STATE=1,INS_VP_STATE=2};

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * u2=(double*)GetInPortPtrs(block,2);
    double * u3=(double*)GetInPortPtrs(block,3);
    double * y1=(double*)GetOutPortPtrs(block,1);
    double * y2=(double*)GetOutPortPtrs(block,2);
    int * ipar=block->ipar;

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
            
    int & mode = ipar[0];

    // static data
    static double ** H_mag;
    static double R_mag_n[3][3];
    static double H_mag_full[10][3];
    static double H_mag_att[4][3];

    //handle flags
    if (flag==scicos::computeOutput)
    {
        double sigDec2 = sigDec*sigDec;
        double sigDip2 = sigDip*sigDip;
        double cosDec = cos(dec), sinDec = sin(dec);
        double cosDec2 = cosDec*cosDec, sinDec2 = sinDec*sinDec;
        double cosDip = cos(dip), sinDip = sin(dip);
        double cosDip2 = cosDip*cosDip, sinDip2 = sinDip*sinDip;
        double Bn = cosDec*cosDip;
        double Be = sinDec*cosDip;
        double Bd = sinDip;

        // we can use the same file for both modes
        // this works since non zero elements are ignored and 
        // the states are the first 4 (quaternions in ATT mode)
        #include "navigation/ins_H_mag.hpp" 
        #include "navigation/ins_R_mag_n.hpp" 
    }
    else if (flag==scicos::terminate)
    {
    }
    else if (flag==scicos::initialize || flag==scicos::reinitialize)
    {
        // determine sizes
        if (mode == INS_FULL_STATE)
        {
            H_mag = &(H_mag_full[0]);
        }
        else if (mode == INS_ATT_STATE)
        {
            H_mag = &(H_mag_att[0]);
        }
        else
        {
            Coserror((char *)"unknown mode for magMeasModel block");
            return;
        }
    }
    else
    {
        std::cout << "unhandled block flag: " << flag << std::endl;
    }
}

} // extern c

// vim:ts=4:sw=4:expandtab
