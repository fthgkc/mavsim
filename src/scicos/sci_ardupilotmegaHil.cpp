/*
 * sci_ardupilotmegaHil.cpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * sci_ardupilotmegaHil.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sci_ardupilotmegaHil.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "utilities.hpp"
#include "communication/AsyncSerial.hpp"
#include <iostream>

#include <ardupilotmegacomm/ArdupilotmegaHil.hpp>

extern "C"
{

#include <scicos/scicos_block4.h>
#include <math.h>
#include "definitions.hpp"

    void sci_ardupilotmegaHil(scicos_block *block, scicos::enumScicosFlags flag)
    {
		using namespace ardupilotmegacomm;

        // data
        double * x=GetRealInPortPtrs(block,1);
        double * u=GetRealOutPortPtrs(block,1);
        int * ipar=block->ipar;
		static char * device;
		static int baudRate;
        static char ** stringArray;
        static int * intArray;

        // serial port
        static ArdupilotmegaHil * hil = NULL;


        //handle flags
        if (flag==scicos::initialize || flag==scicos::reinitialize)
        {
            if (!hil)
			{
				getIpars(1,1,ipar,&stringArray,&intArray); 
				device = stringArray[0];
				baudRate = intArray[0];	
				hil = new ArdupilotmegaHil(device,baudRate);
			}
        }
        else if (flag==scicos::terminate)
        {
            if (hil)
            {
                delete hil;
                hil = NULL;
            }
        }
        else if (flag==scicos::updateState)
        {
        }
        else if (flag==scicos::computeDeriv)
        {
        }
        else if (flag==scicos::computeOutput)
        {
			hil->toApm.msg.roll = x[0];
			hil->toApm.msg.pitch = x[1];
			hil->toApm.msg.heading = x[2];
			hil->toApm.msg.airspeed = x[3];
			hil->send();

			hil->receive();
			u[0] = hil->fromApm.msg.rollServo;
            u[1] = hil->fromApm.msg.pitchServo;
            u[2] = hil->fromApm.msg.throttleServo;
            u[3] = hil->fromApm.msg.rudderServo;
        }
        else
        {
        }
    }

} // extern c

// vim:ts=4:sw=4
