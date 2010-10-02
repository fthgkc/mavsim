/*
 * JSBSimComm.cpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * JSBSimComm.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JSBSimComm.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "JSBSimComm.hpp"
#include "JSBSim/models/FGAuxiliary.h"
#include "JSBSim/models/FGPropulsion.h"
#include "JSBSim/models/FGAircraft.h"
#include "JSBSim/models/FGAtmosphere.h"
#include "JSBSim/models/propulsion/FGEngine.h"
#include "JSBSim/models/propulsion/FGThruster.h"

namespace oooark
{

namespace communication
{

// member functions
JSBSimComm::JSBSimComm(const std::string modelPath,
                       const std::string enginePath,
                       const std::string systemsPath,
                       const std::string model)
{
    std::cout << "Loading model" << std::endl;
    LoadModel(modelPath,enginePath,systemsPath,model);
}
// compute state derivative
void JSBSimComm::calculateXd(const double * x, const double * u, double * xd)
{
    using namespace JSBSim;

	setU(u);
	setX(x);
    
    // longitudinal state derivatives
    xd[0] = (GetPropagate()->GetUVW(1)*GetPropagate()->GetUVWdot(1) +
		GetPropagate()->GetUVW(2)*GetPropagate()->GetUVWdot(2) + 
		GetPropagate()->GetUVW(3)*GetPropagate()->GetUVWdot(3))/ 
		GetAuxiliary()->GetVt(); // from lewis, vtrue dot
    xd[1] = GetAuxiliary()->Getadot();  // alpha dot
    xd[2] = GetAuxiliary()->GetEulerRates(2); // theta dot
    xd[3] = GetPropagate()->GetPQRdot(2); // q dot
    xd[4] = GetPropagate()->Gethdot(); // altitude dot

    // lateral state derivatives
    xd[5] = GetAuxiliary()->Getbdot(); // beta dot
    xd[6] = GetAuxiliary()->GetEulerRates(1); // phi dot
    xd[7] = GetPropagate()->GetPQRdot(1); // p dot 
    xd[8] = GetPropagate()->GetPQRdot(3); // r dot 
    xd[9] = GetAuxiliary()->GetEulerRates(3); // psi dot

    // get rpm derivative
	double rpm0 = GetPropulsion()->GetEngine(0)->GetThruster()->GetRPM();
    Run(); // propagate engine dynamics by deltaT
    xd[10] = (GetPropulsion()->GetEngine(0)->GetThruster()->GetRPM()-rpm0)/propulsionDeltaT;
	// approximation of rpm derivative

    //for (int i=0;i<11;i++) std::cout << "xd[" << i << "]: " << xd[i] << std::endl;
}

void JSBSimComm::calculateY(const double * x, const double * u, double * y, double * xOut)
{
    using namespace JSBSim;
	getY(y);	
    for (int i=0;i<11;i++) xOut[i] = x[i];
}

JSBSimComm::~JSBSimComm()
{
}

void JSBSimComm::setX(const double * x)
{
	using namespace JSBSim;
	// send to jsbsim
    GetIC()->SetVtrueFpsIC(x[0]);
    GetIC()->SetAlphaRadIC(x[1]);
    GetIC()->SetThetaRadIC(x[2]);
    GetIC()->SetQRadpsIC(x[3]);
    GetIC()->SetAltitudeASLFtIC(x[4]); // can't be at zero or we are in the ground
    GetIC()->SetBetaRadIC(x[5]);
    GetIC()->SetPhiRadIC(x[6]);
    GetIC()->SetPRadpsIC(x[7]);
    GetIC()->SetRRadpsIC(x[8]);
    GetIC()->SetPsiRadIC(x[9]);
	for (int i=0;i<GetPropulsion()->GetNumEngines();i++)
    {
        GetPropulsion()->GetEngine(i)->GetThruster()->SetRPM(x[10]);
    }
	// apply
	RunIC();
}

void JSBSimComm::getX(double *x)
{
	using namespace JSBSim;
	x[0] = GetAuxiliary()->GetAeroUVW(1);
	x[1] = GetAuxiliary()->Getalpha();
	x[2] = GetPropagate()->GetEuler(2);
	x[3] = GetAuxiliary()->GetEulerRates(2);
	x[4] = GetPropagate()->GetAltitudeASL();
	x[5] = GetAuxiliary()->Getbeta();
	x[6] = GetPropagate()->GetEuler(1);
	x[7] = GetAuxiliary()->GetEulerRates(1);
	x[8] = GetAuxiliary()->GetEulerRates(3);
	x[9] = GetPropagate()->GetEuler(3);
	x[10] = GetPropulsion()->GetEngine(0)->GetThruster()->GetRPM();
}

void JSBSimComm::setU(const double * u)
{
	using namespace JSBSim;
  	// copy control input to jsbsim
    GetFCS()->SetDaCmd(u[0]); //aileron
    GetFCS()->SetDeCmd(u[1]); //elevator
    GetFCS()->SetDrCmd(u[2]); //rudder
    for (int i=0;i<GetPropulsion()->GetNumEngines();i++)
    {
        FGEngine * engine = GetPropulsion()->GetEngine(i);
        FGThruster * thruster = engine->GetThruster();
        GetFCS()->SetThrottleCmd(i,u[3]);
        thruster->SetdeltaT(propulsionDeltaT);
    }
	// apply
    RunIC();
}

void JSBSimComm::getU(double * u)
{
	using namespace JSBSim;
	u[0] = GetFCS()->GetDaCmd();
	u[1] = GetFCS()->GetDeCmd();
	u[2] = GetFCS()->GetDrCmd();
	u[3] = GetFCS()->GetThrottleCmd(0);
}

void JSBSimComm::getY(double * y)
{
	using namespace JSBSim;
    y[0] = GetAircraft()->GetBodyAccel(1);
    y[1] = GetAircraft()->GetBodyAccel(2);
    y[2] = GetAircraft()->GetBodyAccel(3);
    y[3] = GetPropagate()->GetPQR(1);
    y[4] = GetPropagate()->GetPQR(2);
    y[5] = GetPropagate()->GetPQR(3);
    y[6] = GetAtmosphere()->GetPressure();
    y[7] = GetAuxiliary()->GetTotalPressure();
}

} // oooark

} // communication

// vim:ts=4:sw=4
