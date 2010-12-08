/*
 * sci_gpsIns.cpp
 * Copyright (C) Brandon Wampler 2010 <bwampler@users.sourceforge.net>
 *
 * sci_gpsIns.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sci_gpsIns.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * x: Longitude, Latitude, Altitude, Roll, Pitch, Yaw, Vn, Ve, Vd
 *
 * u1: fbx, fby, fbz, wbx, wby, wbz
 * u2: Lat, Lon, Alt, roll, pitch, yaw, Vn, Ve, Vd
 *
 * y: x
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

    void sci_gpsIns(scicos_block *block, scicos::enumScicosFlags flag)
    {
		oooark::GpsIns* gpsIns = NULL;
        static double * doubleArray;

		double *lat;
		double *lon;
		double *height;
		double *roll;
		double *pitch;
		double *yaw;
		double *sigmaPos;
		double *sigmaAlt;
		double *sigmaVel;
		double *sigmaAccelG;
		double *sigmaGyro;
		bool useGravity;

		double *fbx, *fby, *fbz;
		double *wbx, *wby, *wbz;
		double *Vn, *Ve, *Vd;

        // data
        double *u1=(double*)GetInPortPtrs(block,1);
		double *u2=(double*)GetInPortPtrs(block,2)
        double *xOut=(double*)GetOutPortPtrs(block,1);
        double *y=(double*)GetOutPortPtrs(block,2);
        double *x=(double*)GetState(block);
        double *xd=(double*)GetDerState(block);
        double * rpar=block->rpar;

        // make sure you have initialized the block
        if (!gpsIns && flag!=scicos::initialize)
        {
            sci_gpsIns(block,scicos::initialize);
        }

        //handle flags
        if (flag==scicos::initialize || flag==scicos::reinitialize)
        {
            if (!gpsIns)
            {
				lat = u2[0];
				lon = u2[1];
				height = u2[2];
				roll = u2[3];
				pitch = u2[4];
				yaw = u2[5];
				Vn = u2[6];
				Ve = u2[7];
				Vd = u2[8];
				sigmaPos = rPar[0];
				sigmaAlt = rPar[1];
				sigmaVel = rPar[2];
				sigmaAccelG = rPar[3];
				sigmaGyro = rPar[4];
				useGravity = false;
				
				fbx = u1[0];
				fby = u1[1];
				fbz = u1[2];
				wbx = u1[3];
				wby = u1[4];
				wbz = u1[5];

				try
				{
                	gpsIns = new oooark::GpsIns(*lat,*lon,*height,*roll,*pitch,*yaw,*Vn,*Ve,*Vd,*sigmaPos,*sigmaAlt,*sigmaVel,*sigmaAccelG,*sigmaGyro,useGravity);
				}
				catch (const std::runtime_error & e)
				{
					Coserror((char *)e.what());
				}
            }
        }
        else if (flag==scicos::terminate)
        {
            if (gpsIns)
            {
                delete gpsIns;
                gpsIns = NULL;
            }
        }
        else if (flag==scicos::updateState)
        {
			gpsIns->updateAll(*fbx, *fby, *fbz, *wbx, *wby, *wbz, *lat, *lon, *alt, *Vn, *Ve, *Vd)
        }
        //else if (flag==scicos::computeDeriv)
        //{
        //}
        else if (flag==scicos::computeOutput)
        {
			gpsIns->getState(xOut);
        }
        else
        {
            std::cout << "unhandled flag: " << flag << std::endl;
        }

    }

} // extern c

// vim:ts=4:sw=4
