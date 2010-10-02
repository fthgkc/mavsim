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

#include <iostream>

#include "ApmProtocol.hpp"

namespace oooark
{
	ApmProtocol::ApmProtocol(const std::string & device, const long int baudRate) :
		roll(), pitch(), yaw(), lat(40.430896), lon(-86.914602), altMsl(100), groundSpeed(), groundCourse(), timeOfWeek(), 
		nextWpAlt(),  wpIndex(), controlMode(),
		serial(device,baudRate), step(0),
		message(), header(), payload(), messageOut(), checkSum()
	{
		header.push_back(0x34);
		header.push_back(0x44);
		checkSum.push_back(0x00);
		checkSum.push_back(0x00);
	}
	 
	ApmProtocol::~ApmProtocol(){}

	// read new data
	void ApmProtocol::read()
	{
		std::vector<char> buffer = serial.read();
		//std::cout<<"Buffer Size: "<<buffer.size()<<std::endl;
		if(buffer.size()==0) return;
		for(int i=0;i<buffer.size();i++) message.push_back(buffer[i]);

		while(message.size()>0)
		{
			// look for header
			if (step<header.size())
			{
				if(message.front() == header[step]) step++;
				else step = 0;
				message.pop_front();
			}
			
			else if (step==header.size())
			{
				payloadLength = message.front();
				message.pop_front();
				step++;
			}
			
			else if(step==header.size()+1)
			{
				messageId = message.front();
				message.pop_front();
				step++;
			}
			
			else if(step==header.size()+2)
			{
				messageVersion = message.front();
				message.pop_front();
				step++;
			}

			else if(step>=header.size()+3 && step<payloadLength+5)
			{
				payload.push_back(message.front());
				message.pop_front();
				step++;
			}

			else if(step==payloadLength+5)
			{
				checkSum[0]=message.front();
				message.pop_front();
				step++;
			}
			
			else if(step==payloadLength+6)
			{
				checkSum[1]=message.front();
				message.pop_front();

				// compute checksum
				uint8_t ck_a = 0, ck_b = 0;
					ck_a += payloadLength;
					ck_b += ck_a; 
					ck_a += messageId;
					ck_b += ck_a; 
					ck_a += messageVersion;
					ck_b += ck_a; 
				for (int i=0; i<payload.size();i++)
				{
					ck_a += payload[i];
					ck_b += ck_a; 
				}
				if (ck_a==checkSum[0] && ck_b==checkSum[1])
				{
					
					// message complete, read into packet
					decode();
					//print();
					//std::cout << "message emptying" << std::endl;
					payload.clear();
				}
				else step=0, payload.clear();
			}
		}
	}

	// if message complete, then decode
	void ApmProtocol::decode()
	{
		std::cout<<"Mesage Id: "<<(int)messageId<<std::endl;
		if(messageId==0x02) decodeAttitude();
		else if(messageId==0x03) decodeLocation();
		else if(messageId==0x06) decodePerformance();
		newNavData=true;
	}

	void ApmProtocol::decodeAttitude()
	{
		roll = (*(int16_t *)&payload[0])/100.0*M_PI/180.0;
		pitch = (*(int16_t *)&payload[2])/100.0*M_PI/180.0;
		yaw = (*(int16_t *)&payload[4])/100.0*M_PI/180.0;
	}

	void ApmProtocol::decodeLocation()
	{
		for (int i=0;i<payload.size();i++) locationData.bytes[i]=payload[i]; 
		lat = locationData.msg.lat/1e7;
		lon = locationData.msg.lon/1e7;
		altMsl = locationData.msg.altMsl/10.0;
		groundSpeed = locationData.msg.groundSpeed/100.0;
		groundCourse = locationData.msg.groundCourse/100.0;
		timeOfWeek = locationData.msg.timeOfWeek/1e3;

		//lat = (*(int32_t *)&payload[0])/1e7;
		//lon = (*(int32_t *)&payload[2])/1e7;
		//altMSL = (*(int16_t *)&payload[4])/10.0;
		//groundSpeed = (*(uint16_t *)&payload[0])/100.0;
		//groundCourse = (*(int16_t *)&payload[2])/100.0;
		//timeOfWeek = (*(uint32_t *)&payload[4])/1e3;
		//std::cout<<"Lat(deg): "<<lat<<"  Lon(deg): "<<lon<<"  AltMSL(m): "<<altMSL<<std::endl;
		//std::cout<<"Speed(m/s): "<<lat<<"  Course(deg): "<<lon<<"  Time of week(sec): "<<altMSL<<std::endl;
	}

	void ApmProtocol::decodePerformance()
	{
		//TODO  Decode Performance data
	}

	void ApmProtocol::write(double * x)
	{
		//// add header
		//for (int i=0;i<header.size();i++) messageOut.push_back(header[i]);

		//// add xplane packet
		//int16_t rollOut = x[6]*180.0/M_PI*100;
		//int16_t pitchOut = x[2]*180.0/M_PI*100;
		//int16_t headingOut = x[9]*180.0/M_PI*100;
		//int16_t vtOut = x[0]*3.2808399*100;
		//std::cout << "roll out: " << rollOut << std::endl;
		//std::cout << "pitch out: " << pitchOut << std::endl;
		//std::cout << "heading out: " << headingOut << std::endl;
		//std::cout << "vt out: " << vtOut << std::endl;
		//messageOut.push_back(8);
		//messageOut.push_back(0x04); // xplane packet	
		//messageOut.push_back((uint8_t)(rollOut));
		//messageOut.push_back((uint8_t)(rollOut >> 8));	
		//messageOut.push_back((uint8_t)(pitchOut));	
		//messageOut.push_back((uint8_t)(pitchOut >> 8));	
		//messageOut.push_back((uint8_t)(headingOut));	
		//messageOut.push_back((uint8_t)(headingOut >> 8));
		//messageOut.push_back((uint8_t)(vtOut));	
		//messageOut.push_back((uint8_t)(vtOut >> 8));	

		//// compute checksum
		//uint8_t ck_a = 0, ck_b = 0;
		//for (int i=header.size();i<messageOut.size();i++)
		//{
			//ck_a += messageOut[i];
			//ck_b += ck_a; 
		//}
		//messageOut.push_back(ck_a);
		//messageOut.push_back(ck_b);

		//// output to serial
		//serial.write(messageOut);
		//messageOut.clear();
	}

	void ApmProtocol::print()
	{
		//std::cout
			//<< "\nroll:\t\t\t" << roll
			//<< "\npitch:\t\t\t" << pitch
			//<< "\nthrottle:\t\t" << throttle
			//<< "\nrudder:\t\t\t" << rudder
			//<< "\nwaypoint distance:\t" << wpDistance
			//<< "\nbearing errror:\t\t" << bearingError
			//<< "\nnext waypoint alt:\t" << nextWpAlt
			//<< "\nenergy error:\t\t" << energyError
			//<< "\nwaypoint index:\t\t" << wpIndex
			//<< "\ncontrol mode:\t\t" << controlMode
			//<< std::endl;
	}
}//end namespace oooark
