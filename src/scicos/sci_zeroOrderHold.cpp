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

void sci_zeroOrderHold(scicos_block *block,int flag)
{
    double *_z=GetDstate(block);
    double *_u1=GetRealInPortPtrs(block,1);
    double *_u2=GetRealInPortPtrs(block,2);
    double *_y1=GetRealOutPortPtrs(block,1);
  
    int i;
    for(i=0;i< GetInPortRows(block,1);i++){
        if (flag ==1 || flag ==6 || flag ==4)
            _y1[i]=_z[i];
        else if (flag == 2)
        {
            switch(GetNevIn(block))
            {
                case 1: _z[i]=_u1[i]; break;
                case 2: _z[i]=_u2[i]; break;
            }
        }
    }
}

}

// vim:ts=4:sw=4:expandtab
