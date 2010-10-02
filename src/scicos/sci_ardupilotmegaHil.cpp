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

class ArdupilotmegaHil
{
public:
	// commmunication
	BufferedAsyncSerial serial;
	int headerCount;
	bool headerFound;
	std::vector<char> message;
	std::vector<char> header;
	std::vector<char> headerOut;
	std::vector<char> messageOut;

	// constants
	static const int packetLength = 18;

	struct XPlanePacket
	{
		int16_t roll, pitch, throttle, rudder, wpDistance, bearingError;
		int16_t nextWpAlt, energyError;
		uint8_t wpIndex, controlMode;
	};

	union
	{
		XPlanePacket msg;
		uint8_t bytes[];
	} xPlanePacket;

	// constructor
	ArdupilotmegaHil(const std::string & device, const int baudRate) :
		serial(device,baudRate), headerCount(0), headerFound(false),
		message(), header()
	{
		header.push_back('A');
		header.push_back('A');
		header.push_back('A');

		headerOut.push_back(0x44);
		headerOut.push_back(0x49);
		headerOut.push_back(0x59);
		headerOut.push_back(0x64);
	}
	
	// read new data
	void read(double * u)
	{
		//std::cout << "reading" << std::endl;
		std::vector<char> buffer = serial.read();

		int i = 0;

		// look for header
		if (!headerFound)
		{
			while(i<buffer.size())
			{
				if(buffer[i++] == header[headerCount]) headerCount++;
				else headerCount = 0;

				// read message
				if (headerCount >= header.size())
				{
					//std::cout << "header found" << std::endl;
					headerCount = 0;
					headerFound = true;
					break;
				}
			}
		}

		// read message
		if (headerFound)
		{
			std::cout << "header found" << std::endl;
			while(i<buffer.size()) message.push_back(buffer[i++]);
			if (message.size() == packetLength)
			{
				std::cout << "message found" << std::endl;
				// message complete, read into packet
				//
				for (int i=0;i<packetLength;i++) xPlanePacket.bytes[i] = message[i];
				print();
				headerFound = false;
				//std::cout << "message emptying" << std::endl;
				message.clear();
			}
		}

		// set u
		u[0] = xPlanePacket.msg.throttle;
		u[1] = xPlanePacket.msg.roll;
		u[2] = xPlanePacket.msg.pitch;
		u[3] = xPlanePacket.msg.rudder;
	}

	void write(double * x)
	{
		// add header
		for (int i=0;i<headerOut.size();i++) messageOut.push_back(headerOut[i]);

		// add xplane packet
		int16_t rollOut = x[6]*180.0/M_PI*100;
		int16_t pitchOut = x[2]*180.0/M_PI*100;
		int16_t headingOut = x[9]*180.0/M_PI*100;
		int16_t vtOut = x[0]*3.2808399*100;
		std::cout << "roll out: " << rollOut << std::endl;
		std::cout << "pitch out: " << pitchOut << std::endl;
		std::cout << "heading out: " << headingOut << std::endl;
		std::cout << "vt out: " << vtOut << std::endl;
		messageOut.push_back(8);
		messageOut.push_back(0x04); // xplane packet	
		messageOut.push_back((uint8_t)(rollOut));
		messageOut.push_back((uint8_t)(rollOut >> 8));	
		messageOut.push_back((uint8_t)(pitchOut));	
		messageOut.push_back((uint8_t)(pitchOut >> 8));	
		messageOut.push_back((uint8_t)(headingOut));	
		messageOut.push_back((uint8_t)(headingOut >> 8));
		messageOut.push_back((uint8_t)(vtOut));	
		messageOut.push_back((uint8_t)(vtOut >> 8));	

		// compute checksum
		uint8_t ck_a = 0, ck_b = 0;
		for (int i=headerOut.size();i<messageOut.size();i++)
		{
			ck_a += messageOut[i];
			ck_b += ck_a; 
		}
		messageOut.push_back(ck_a);
		messageOut.push_back(ck_b);

		// output to serial
		serial.write(messageOut);
		messageOut.clear();
	}

	void print()
	{
		std::cout
			<< "\nroll:\t\t\t" << xPlanePacket.msg.roll
			<< "\npitch:\t\t\t" << xPlanePacket.msg.pitch
			<< "\nthrottle:\t\t" << xPlanePacket.msg.throttle
			<< "\nrudder:\t\t\t" << xPlanePacket.msg.rudder
			<< "\nwaypoint distance:\t" << xPlanePacket.msg.wpDistance
			<< "\nbearing errror:\t\t" << xPlanePacket.msg.bearingError
			<< "\nnext waypoint alt:\t" << xPlanePacket.msg.nextWpAlt
			<< "\nenergy error:\t\t" << xPlanePacket.msg.energyError
			<< "\nwaypoint index:\t\t" << xPlanePacket.msg.wpIndex
			<< "\ncontrol mode:\t\t" << xPlanePacket.msg.controlMode
			<< std::endl;
	}
};

extern "C"
{

#include <scicos/scicos_block4.h>
#include <math.h>
#include "definitions.hpp"

    void sci_ardupilotmegaHil(scicos_block *block, scicos::enumScicosFlags flag)
    {
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
            hil->write(x);
            hil->read(u);
        }
        else
        {
        }
    }

} // extern c

// vim:ts=4:sw=4
