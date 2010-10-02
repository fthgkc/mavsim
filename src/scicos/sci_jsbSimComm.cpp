/*
 * sci_jsbSimComm.cpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * sci_jsbSimComm.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sci_jsbSimComm.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "JSBSim/FGFDMExec.h"
#include "JSBSim/models/FGFCS.h"
#include <iostream>
#include <string>
#include "communication/JSBSimComm.hpp"

extern "C"
{

#include <scicos/scicos_block4.h>
#include <math.h>
#include "definitions.hpp"

    void sci_jsbSimComm(scicos_block *block, scicos::enumScicosFlags flag)
    {
        //definitions
        static oooark::communication::JSBSimComm * jsbSim = NULL;

        // data
        double *u=(double*)GetInPortPtrs(block,1);
        double *xOut=(double*)GetOutPortPtrs(block,1);
        double *y=(double*)GetOutPortPtrs(block,2);
        double *x=(double*)GetState(block);
        double *xd=(double*)GetDerState(block);

        //handle flags
        if (flag==scicos::initialize || flag==scicos::reinitialize)
        {
            std::cout << "initializing" << std::endl;
            if (jsbSim) delete jsbSim; //delete if already allocated
            jsbSim = new oooark::communication::JSBSimComm(std::string(getenv("OOOARK"))+
                    "data/","/Engines","/systems","EasyStar");
        }
        else if (flag==scicos::terminate)
        {
            std::cout << "terminating" << std::endl;
            if (jsbSim) delete jsbSim; //delete if already allocated
            jsbSim=NULL;
        }
        else if (flag==scicos::updateState)
        {
            // make sure we have initialized first
            if (!jsbSim) sci_jsbSimComm(block,scicos::initialize);

            // update the state
            //jsbSim->updateState(x);
        }
        else if (flag==scicos::computeDeriv)
        {
            // make sure we have initialized first
            if (!jsbSim) sci_jsbSimComm(block,scicos::initialize);

            // update the state
            //sci_jsbSimComm(block,scicos::updateState);

            // run the communications
            jsbSim->calculateXd(x,u,xd);
        }
        else if (flag==scicos::computeOutput)
        {
            // make sure we have initialized first
            if (!jsbSim) sci_jsbSimComm(block,scicos::initialize);

            // calculate the output
            jsbSim->calculateY(x,u,y,xOut);
        }
        else
        {
            std::cout << "unhandled flag: " << flag << std::endl;
        }
    }

} // extern c

// vim:ts=4:sw=4
