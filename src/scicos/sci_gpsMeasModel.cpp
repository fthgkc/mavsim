/*sci_gpsMeasModel.cpp
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
 * USAGE:
 *
 * input 1:
 *   [1] h (altitude) (unit distance, set by real parameter R)
 *   [2] L (latitude) (radians)
 *
 * input 2:
 *   [1] velocity error std deviation (unit distance/second)
 *   [2] position error std deviation (unit distance)
 *   [2] altitude error std deviation (unit distance)
 *
 * output 1: 
 *
 *  mode (0)
 *   H_gps (6x10)
 *
 *  mode (1)
 *    attitude state not allowed, since H would be zero
 *
 *  mode (2)
 *   H_gps (6x6)
 *
 * output 2:
 *   R_gps (6x6)
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

void sci_gpsMeasModel(scicos_block *block, scicos::enumScicosFlags flag)
{
    enum ins_mode {INS_FULL_STATE=0,INS_ATT_STATE=1,INS_VP_STATE=2};

    // constants

    // data
    double * u1=(double*)GetInPortPtrs(block,1);
    double * u2=(double*)GetInPortPtrs(block,2);
    double * y1=(double*)GetOutPortPtrs(block,1);
    double * y2=(double*)GetOutPortPtrs(block,2);
    int * ipar=block->ipar;
    double * rpar=block->rpar;

    // alias names
    double & h = u1[0];
    double & L = u1[1];
    double & sigVel = u2[0];
    double & sigPos = u2[1];
    double & sigAlt = u2[2];

    int & mode = ipar[0];
    double & Re = rpar[0];

    // static data
    static double ** H_gps, ** R_gps;
    static int nX;
    static const int nZ = 3;

    //handle flags
    if (flag==scicos::computeOutput)
    {
        if ( ! (H_gps && R_gps) )
        {
            Coserror((char *)"gpsMeasModel block initialization failed.");
            return;
        }
        const double cosL = cos(L);
        const double sigVel2 = sigVel*sigVel;
        const double sigPos2 = sigPos*sigPos;
        const double sigAlt2 = sigAlt*sigAlt;
        const double R = R*R;

        if (mode == INS_FULL_STATE)
        {
            #include "navigation/ins_H_gps.hpp" 
        }
        else if (mode == INS_VP_STATE)
        {
            #include "navigation/ins_H_gps_vp.hpp" 
        }
        else
        {
            Coserror((char *)"unknown mode for gpsMeasModel block");
            return;
        }

        // set output vectors
        cMatrix2FortranVector(H_gps,nZ,nX,y1);
        cMatrix2FortranVector(R_gps,nZ,nZ,y2);
    }
    else if (flag==scicos::terminate)
    {
        freeCMatrix(H_gps,nZ,nX);
        freeCMatrix(R_gps,nZ,nZ);
    }
    else if (flag==scicos::initialize || flag==scicos::reinitialize)
    {
        // prevent reinitialization
        if (H_gps || R_gps) return; 

        // determine sizes
        if (mode == INS_FULL_STATE)
        {
            nX = 10;
        }
        else if (mode == INS_VP_STATE)
        {
            nX = 6;
        }
        else
        {
            Coserror((char *)"unknown mode for insErrorDynamics block");
            return;
        }

        // allocate matrices
        allocateCMatrix(H_gps,nZ,nX);
        allocateCMatrix(R_gps,nZ,nZ);
    }
    else
    {
        std::cout << "unhandled block flag: " << flag << std::endl;
    }
}

} // extern c

// vim:ts=4:sw=4:expandtab
