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
//#include "oooark/utilities/utilities.hpp"
#include <deque>
#include "AsyncSerial.hpp"

namespace oooark
{

class ApmProtocol
{
public:
	// data
	double roll, pitch, yaw;
	double lat, lon, altMsl, groundSpeed, groundCourse, timeOfWeek;
	double nextWpAlt;
	int wpIndex, controlMode;

	double reportInterval, maxLoopTime, imuHealth;
	uint16_t mainLoopCycles, gcsMessageSentCount, gyroSatCount, adcConstCount, renormSqrtCount, renormBlowupCount, gpsFixCount;
	

	// commmunication
	BufferedAsyncSerial serial;
	int step;
	bool newNavData;
	std::vector<char> header;
	std::deque<char> message;
	std::vector<char> payload;
	std::vector<char> messageOut;
	std::vector<char> checkSum;
	char payloadLength, messageId, messageVersion;

	// constants
    ApmProtocol(const std::string & device, const long int baudRate);
	
    // destructors
    virtual ~ApmProtocol();
	
	void read();	
	
	struct LocationData {
		int32_t		lat;
		int32_t		lon;
		int16_t		altMsl;
		uint16_t		groundSpeed;
		int16_t		groundCourse;
		uint32_t	timeOfWeek;
	};
	union {
		LocationData	msg;
		uint8_t			bytes[];
	} locationData;

	
	void decode();
	void decodeAttitude();
	void decodeLocation();
	void decodePerformance();
	void write(double * x);
	void print();
};

} // oooark

#endif

// vim:ts=4:sw=4
