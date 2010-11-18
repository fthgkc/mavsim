/*
 * sci_mavlinkHil.cpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * sci_mavlinkHil.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sci_mavlinkHil.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "utilities.hpp"
#include "communication/AsyncSerial.hpp"
#include "communication/asio_mavlink_bridge.h"
#include "common/mavlink.h"
#include <iostream>
#include <stdexcept>

extern "C"
{
#include <scicos/scicos_block4.h>
#include <math.h>
#include "definitions.hpp"

    void sci_mavlinkHil(scicos_block *block, scicos::enumScicosFlags flag)
    {
        // data
        double * x=GetRealInPortPtrs(block,1);
        double * u=GetRealOutPortPtrs(block,1);
        int * ipar=block->ipar;

        static char * device;
        static int baudRate;
        static char ** stringArray;
        static int * intArray;

        static uint16_t packet_drops = 0;
        static mavlink_rc_channels_scaled_t rc_channels;

        //handle flags
        if (flag==scicos::initialize || flag==scicos::reinitialize)
        {
            if (mavlink_comm_0_port == NULL)
            {
                getIpars(1,1,ipar,&stringArray,&intArray);
                device = stringArray[0];
                baudRate = intArray[0];
                try
                {
                    mavlink_comm_0_port = new BufferedAsyncSerial(device,baudRate);
                }
                catch(const boost::system::system_error & e)
                {
                    Coserror((char *)e.what());
                }
            }
        }
        else if (flag==scicos::terminate)
        {
            if (mavlink_comm_0_port)
            {
                delete mavlink_comm_0_port;
                mavlink_comm_0_port = NULL;
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
            mavlink_message_t msg;
            mavlink_status_t status;

            while(comm_get_available(MAVLINK_COMM_0))
            {
                uint8_t c = comm_receive_ch(MAVLINK_COMM_0);

                // try to get new message
                if(mavlink_parse_char(MAVLINK_COMM_0,c,&msg,&status))
                {
                    switch(msg.msgid)
                    {
                        case MAVLINK_MSG_ID_RC_CHANNELS_SCALED:
                        {
                            mavlink_msg_rc_channels_scaled_decode(&msg,&rc_channels);
                            u[0] = rc_channels.chan1_scaled/10000.0f;
                            u[1] = rc_channels.chan2_scaled/10000.0f;
                            u[2] = rc_channels.chan3_scaled/10000.0f;
                            u[3] = rc_channels.chan4_scaled/10000.0f;
                            u[4] = rc_channels.chan5_scaled/10000.0f;
                            u[5] = rc_channels.chan6_scaled/10000.0f;
                            u[6] = rc_channels.chan7_scaled/10000.0f;
                            u[7] = rc_channels.chan8_scaled/10000.0f;
                            break;
                        }
                    }
                }

                // update packet drop counter
                packet_drops += status.packet_rx_drop_count;
            }
        }
        else
        {
        }
    }

} // extern c

// vim:ts=4:sw=4
