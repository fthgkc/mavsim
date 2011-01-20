/*sci_insErrorDynamics.cpp
 * Copyright (C) Alan Kim 2011 
 * 
 * sci_insErrorDynamics.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sci_insErrorDynamics.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * u1: fn, fe, fd
 * u2: Lat, roll, pitch, yaw, Vn, Ve, Vd
 *
 * Out1 = F (9x9)
 * Out2 = G (9x9)
 *
 * for del_x_dot = F*del_x + G*u
 * where u del_x is the error state of INS
 * and u is the inputs to the INS error dynamics
 *
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include "math/GpsIns.hpp"
#include "utilities.hpp"
#include <stdexcept>

extern "C"
{

#include <scicos/scicos_block4.h>
#include <math.h>
#include "definitions.hpp"

    void sci_insErrorDynamics(scicos_block *block, scicos::enumScicosFlags flag)
 {

		// constants
		

	 	// data
        double * u1=(double*)GetInPortPtrs(block,1);
	double * u2=(double*)GetInPortPtrs(block,2);
        double ** F=(double**)GetOutPortPtrs(block,1);
	double ** G=(double**)GetOutPortPtrs(block,2);
        double * rpar=block->rpar;

		// alias names
		double & fn = u1[0];
		double & fe = u1[1];
		double & fd = u1[2];

		double & lat = u2[0];
		double & roll = u2[1];
		double & pitch = u2[2];
		double & yaw = u2[3];
		double & Vn = u2[4];
		double & Ve = u2[5];
		double & Vd = u2[6];

		double & Omega = rpar[0];
		double & R = rpar[1];
				
			
       //handle flags

        if (flag==scicos::terminate)
        {
            std::cout << "terminating" << std::endl;
        }
        else if (flag==scicos::computeOutput)
        {
            std::cout << "computing Output" << std::endl;
	    memset((void *)F,0,81*sizeof(double));
	    memset((void *)G,0,81*sizeof(double));
	   
	    double clat = cos(lat);
	    double slat = sin(lat);
	    double tlat = tan(lat);
            double croll = cos(roll);
	    double sroll = sin(roll);
	    double cpitch =  cos(pitch);
	    double spitch = sin(pitch);
	    double cyaw = cos(yaw);
	    double syaw = sin(yaw);

	    F[0][1]=-(Omega*slat+Ve/R*tlat);
	    F[0][2]=Vn/R;
	    F[0][4]=1/R;
	    F[0][6]=-Omega*slat;
	    F[0][8]=-Ve/(R*R);
	    F[1][0]=(Omega*slat+Ve/R*tlat);
	    F[1][2]=Omega*clat+Ve/R;
	    F[1][3]=-1/R;
	    F[1][8]=Vn/(R*R);
	    F[2][0]=-Vn/R;
	    F[2][1]=-Omega*clat-Ve/R;
	    F[2][4]=-tlat/R;
	    F[2][6]=-Omega*clat-Ve/(R*clat*clat);
	    F[2][8]=Ve*tlat/(R*R);
	    F[3][1]=-fd;
	    F[3][2]=fe;
	    F[3][3]=Vd/R;
	    F[3][4]=-2*(Omega*slat+Ve/R*tlat);
	    F[3][5]=Vn/R;
	    F[3][6]=-Ve*(2*Omega*clat+Ve/(R*clat*clat));
	    F[3][8]=1/(R*R)*(Ve*Ve*tlat-Vn*Vd);
	    F[4][0]=fd;
	    F[4][2]=-fn;
	    F[4][3]=(2*Omega*slat+Ve/R*tlat);
	    F[4][4]=1/R*(Vn*tlat+Vd);
	    F[4][5]=2*Omega*cos(lat)+Ve/R;
	    F[4][6]=(2*Omega*(Vn*clat-Vd*slat)+Vn*Ve/(R*clat*clat));
	    F[4][8]=-Ve/(R*R)*(Vn*tlat+Vd);
	    F[5][0]=-fe;
	    F[5][1]=fn;
	    F[5][3]=-2*Vn/R;
	    F[5][4]=-2*(Omega*clat+Ve/R);
	    F[5][6]=2*Omega*Ve*sin(lat);
	    F[5][8]=1/(R*R)*(Vn*Vn+Ve*Ve);
	    F[6][3]=1/R;
	    F[6][8]=-Vn/(R*R);
	    F[7][4]=1/(R*clat);
	    F[7][6]=Ve*tlat/(R*clat);
	    F[7][8]=-Ve/(R*R*clat);
	    F[8][5]=-1;
	
	    G[0][0]=-cpitch*cyaw;
	    G[0][1]=-(-croll*syaw+sroll*spitch*cyaw);
	    G[0][2]=-(sroll*syaw+croll*spitch*cyaw);
	    G[1][0]=-cpitch*syaw;
	    G[1][1]=-(croll*cyaw+sroll*spitch*syaw);
	    G[1][2]=-(-sroll*cyaw+croll*spitch*syaw);
	    G[2][0]=spitch;
	    G[2][1]=-sroll*cpitch;
	    G[2][2]=-croll*cpitch;
	    G[3][3]=-G[0][0];
	    G[3][4]=-G[0][1];
	    G[3][5]=-G[0][2];
	    G[4][3]=-G[1][0];
	    G[4][4]=-G[1][1];
	    G[4][5]=-G[1][2];
	    G[5][3]=-G[2][0];
	    G[5][4]=-G[2][1];
	    G[5][5]=-G[2][2];



	}
        else
        {
            std::cout << "unhandled flag: " << flag << std::endl;
        }

    }

} // extern c

// vim:ts=4:sw=4
