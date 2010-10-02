/*
 * JSBSimComm.hpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * JSBSimComm.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JSBSimComm.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JSBSimComm_H
#define JSBSimComm_H

#include "JSBSim/FGFDMExec.h"
#include "JSBSim/models/FGFCS.h"
#include <iostream>
#include <string>

namespace oooark
{

namespace communication
{

enum { xi_velocityTrue, xi_alpha, xi_theta, xi_q, xi_altitude, xi_beta,
	xi_phi, xi_p, xi_r, xi_psi, xi_rpm };

enum { ui_aileron, ui_elevator, ui_rudder, ui_throttle};

class JSBSimComm : public JSBSim::FGFDMExec
{
private:
    // constants
    const static double pi_2=M_PI/2.;

public:
    // member functions
    JSBSimComm(std::string modelPath,std::string enginePath,
               std::string systemsPath,std::string model);

    // compute state derivative
    void calculateXd(const double * x, const double * u, double * xd);

    void calculateY(const double * x, const double * u, double * y, double * xOut);

    virtual ~JSBSimComm();

	// access
	void getX(double * x);
	void getY(double * y);
	void getU(double * u);
	void setX(const double *x);
	void setU(const double *u);
private:
    // constants
    const static double rad2deg=180./M_PI;
	const static double propulsionDeltaT = 0.01;
};

} // communication

} // oooark

#endif

// vim:ts=4:sw=4
