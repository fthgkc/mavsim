/*
 * sci_zeroOrderHold.cpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * This file free software: you can redistribute it and/or modify it
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
 */

extern "C"

{

#include <scicos/scicos_block4.h>
#include "definitions.hpp"
#include "stdio.h"

void sci_zeroOrderHold(scicos_block *block,scicos::enumScicosFlags flag)
{
    // get block data pointers, etc
    double *_z=GetDstate(block);
    double *_u1=GetRealInPortPtrs(block,1);
    double *_u2=NULL;
    double *_y1=GetRealOutPortPtrs(block,1);
    int *_ipar=GetIparPtrs(block);
    int & evtFlag = GetNevIn(block);

    // conditional data based on size
    if (GetNin(block) == 2) _u2=GetRealInPortPtrs(block,2);

    // compute flags
    int evtFlagTime = 1 << (_ipar[0]-1);
    int evtFlagReset = 1 << (_ipar[1]-1);
  
    // loop over all rows of data
    int i;
    for(i=0;i< GetInPortRows(block,1);i++){
        
        if (flag ==scicos::computeOutput || flag ==scicos::reinitialize || flag ==scicos::initialize)
            _y1[i]=_z[i];

        else if (flag == scicos::updateState)
        {
            if(evtFlag & evtFlagReset) // bitwise comparison for flag
            {
                _z[i]=_u2[i];
            }
            if(evtFlag & evtFlagTime)
            {
                _z[i]=_u1[i];
            }
            else
            {
                char msg[50];
                sprintf(msg,"unhandled event flag %d\n",evtFlag);
                Coserror(msg);
            }
        }
        else if (flag == scicos::terminate)
        {
            printf("terminating");
            end_scicos_sim(); // force termination
        }
        else
        {
            char msg[50];
            sprintf(msg,"unhandled block flag %d\n",flag);
            Coserror(msg);
        }
    }
}

}

// vim:ts=4:sw=4:expandtab
