/*
 * ApmProtocol.hpp
 * Copyright (C) Brandon Wampler 2010 <brandon.wampler@gmail.com>
 *
 * ApmProtocol.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ApmProtocol.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ApmProtocol_h
#define ApmProtocol_h

#include <vector>
#include <apmhsl/APM_BinComm/APM_BinComm.h>
#include <apmhsl/WProgram.h>
namespace oooark
{

class ApmProtocol
{
private:
	BufferedAsyncSerial serial;
	BinComm::MessageHandler handlerTable[10];
	Stream stream;

public:
	ApmProtocol(const std::string & device, long int baud);
	virtual ~ApmProtocol();
	void update();
	void printNav();

	// Comm and data
	BinComm comm;
	int16_t roll, pitch, altMsl;
	uint16_t yaw, groundSpeed, groundCourse;
	int32_t lat, lon;
   	uint32_t timeOfWeek;
	bool newNavData;
	void getNavData(double& roll, double& pitch, double& yaw, double& rLatitude, double& rLongitude, double& rAltMsl, double& rGroundSpeed, double&rGroundCourse, double& rTimeOfWeek);
//double nextWpAlt;
	//int wpIndex, controlMode;
	//double reportInterval, maxLoopTime, imuHealth;
	//uint16_t mainLoopCycles, gcsMessageSentCount, gyroSatCount, adcConstCount, renormSqrtCount, renormBlowupCount, gpsFixCount;
};
} // oooark

#endif

// vim:ts=4:sw=4
