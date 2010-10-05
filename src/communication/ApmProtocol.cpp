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
void h_attitude(void * arg, uint8_t messageId, uint8_t messageVersion, void * messageData);
void h_location(void * arg, uint8_t messageId, uint8_t messageVersion, void * messageData);

ApmProtocol::ApmProtocol(const std::string & device, long int baud) :
		serial(device,baud),
		stream(&serial),
		comm(handlerTable,&stream), newNavData(false), lat(0), lon(0), altMsl(0), groundCourse(0), groundSpeed(0), timeOfWeek(0)
	{
		int i=0;

		handlerTable[i].messageID = BinComm::msg_ATTITUDE;
		handlerTable[i].handler = h_attitude;
		handlerTable[i].arg = this;
		i++;

		handlerTable[i].messageID = BinComm::msg_LOCATION;
		handlerTable[i].handler = h_location;
		handlerTable[i].arg = this;
		i++;

		// signals end of handle table
		handlerTable[i].messageID = BinComm::msg_NULL;
	}	 
	ApmProtocol::~ApmProtocol(){}

	// read new data
	void ApmProtocol::update()
	{
		//comm.update();
	}

	void ApmProtocol::getNavData(double& rRoll, double& rPitch, double& rYaw, double& rLatitude, double& rLongitude, double& rAltMsl, double& rGroundSpeed, double&rGroundCourse, double& rTimeOfWeek)
	{
		rRoll = (roll*M_PI/180)/100.0;
		rPitch = (pitch*M_PI)/180/100.0;
		rYaw = (yaw*M_PI/180)/100.0;
		rLatitude=lat/1e7;
		rLongitude=lon/1e7;
		rAltMsl = altMsl/10.0;
		rGroundSpeed = groundSpeed/100.0;
	   	rGroundCourse = (groundCourse*M_PI/180)/100.0;
		rTimeOfWeek = timeOfWeek/1000.0;	
		newNavData=false;
	}

	void ApmProtocol::printNav()
	{
		std::cout
			<< "lat: " << ((double)lat)/1e7
			<< "\t\tlon: " << ((double)lon)/1e7			
			<< "\t\taltMsl: " << ((double)altMsl)/100.0
			<< "\t\troll: " << ((double)roll)/100.0
			<< "\t\tpitch: " << ((double)pitch)/100.0
			<< "\t\t\tyaw: " << ((double)yaw)/100.0
			<< std::endl;
		newNavData=false;
	}

void h_attitude(void * arg, uint8_t messageId, uint8_t messageVersion, void * messageData)
{
	ApmProtocol* protocol = (ApmProtocol*)arg;
	//protocol->comm.unpack_msg_attitude(protocol->roll,protocol->pitch,protocol->yaw);
	protocol->newNavData=true; 

	//std::cout << "roll, pitch, yaw:\t" << protocol->roll << "\t" << protocol->pitch << "\t" << protocol->yaw << std::endl;
}

void h_location(void * arg, uint8_t messageId, uint8_t messageVersion, void * messageData)
{
	ApmProtocol* protocol = (ApmProtocol*)arg;
	//protocol->comm.unpack_msg_location(protocol->lat,protocol->lon,protocol->altMsl,protocol->groundSpeed,protocol->groundCourse,protocol->timeOfWeek);

	protocol->newNavData=true; 

	//std::cout << "lat, lon, altMsl:\t" << protocol->lat << "\t" 
		//<< protocol->lon << "\t" << protocol->altMsl <<"\t"
		//<< "ground speed, ground course, time of week:\t" << protocol->groundSpeed << "\t" 
		//<< protocol->groundCourse << "\t" << protocol->timeOfWeek << std::endl;
}
}//end namespace oooark
