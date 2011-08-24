/*
 * sci_jsbsimTrim.cpp
 * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 *
 * sci_jsbsimTrim.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * sci_jsbsimTrim.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include "initialization/FGTrimmer.h"
#include "math/FGStateSpace.h"
#include <iomanip>
#include <fstream>
#include "models/FGAircraft.h"
#include "models/propulsion/FGEngine.h"
#include "models/propulsion/FGTurbine.h"
#include "models/propulsion/FGTurboProp.h"
#include "config.h"

template <class varType>
void prompt(const std::string & str, varType & var)
{
    std::cout << str + " [" << std::setw(10) << var << "]\t: ";
    if (std::cin.peek() != '\n')
    {
        std::cin >> var;
        std::cin.ignore(1000, '\n');
    }
    else std::cin.get();
}

void trimFunction ()
{
    using namespace JSBSim;

    // variables
    FGFDMExec fdm;
    fdm.Setdt(1./120);
    FGTrimmer::Constraints constraints;

    std::cout << "\n==============================================\n";
    std::cout << "\tJSBSim Trimming Utility\n";
    std::cout << "==============================================\n" << std::endl;

    // defaults
    constraints.velocity = 45;
    std::string aircraft="easystar-datcom";
    std::string aircraftPath="easystar";
    std::string enginePath="easystar";
    std::string systemsPath="easystar";
    std::string root=datadir;
    double rtol = std::numeric_limits<float>::epsilon();
    double abstol = std::numeric_limits<double>::epsilon();
    double speed = 2.0;
    int iterMax = 2000;
    bool showConvergeStatus = false;
    bool pause = false;
    bool showSimplex = false;
    bool variablePropPitch = false;
    int debugLevel = 0;

    // input
    std::cout << "input ( press enter to accept [default] )\n" << std::endl;

    // load model
    prompt("\tdebug level\t\t",debugLevel);
    fdm.SetDebugLevel(debugLevel);
    std::cout << "model selection" << std::endl;
    while (1)
    {
        prompt("\troot\t\t",root);
        prompt("\taircraft\t\t",aircraft);
        prompt("\taircraft path\t\t",aircraftPath);
        prompt("\tengine path\t\t",enginePath);
        prompt("\tsystems path\t\t",systemsPath);
        fdm.LoadModel(root+"/"+aircraftPath,root+"/"+enginePath,
                      root+"/"+systemsPath,aircraft,false);
        std::string aircraftName = fdm.GetAircraft()->GetAircraftName();
        if (aircraftName == "")
        {
            std::cout << "\tfailed to load aircraft" << std::endl;
        }
        else
        {
            std::cout << "\tsuccessfully loaded: " <<  aircraftName << std::endl;
            break;
        }
    }

    // Turn on propulsion system
    fdm.GetPropulsion()->InitRunning(-1);

    // get propulsion pointer to determine type/ etc.
    FGEngine * engine0 = fdm.GetPropulsion()->GetEngine(0);
    FGThruster * thruster0 = engine0->GetThruster();

    // flight conditions
    std::cout << "\nflight conditions: " << std::endl;
    prompt("\taltitude, ft\t\t",constraints.altitude);
    prompt("\tvelocity, ft/s\t\t",constraints.velocity);
    prompt("\tgamma, deg\t\t",constraints.gamma);
    if (thruster0->GetType()==FGThruster::ttPropeller)
        prompt("\tvariable prop pitch?\t\t",variablePropPitch);
    constraints.gamma *= M_PI/180;

    // mode menu
    while (1)
    {
        int mode = 0;
        prompt("\tmode < non-turning(0), rolling(1), pitching(2), yawing(3) >",mode);
        constraints.rollRate = 0;
        constraints.pitchRate = 0;
        constraints.yawRate = 0;
        if (mode == 0) break;
        else if (mode == 1)
        {
            prompt("\troll rate, rad/s",constraints.rollRate);
            prompt("\tstability axis roll",constraints.stabAxisRoll);
            break;
        }
        else if (mode == 2)
        {
            prompt("\tpitch rate, rad/s",constraints.pitchRate);
            break;
        }
        else if (mode == 3)
        {
            prompt("\tyaw rate, rad/s",constraints.yawRate);
            break;
        }
        else std::cout << "\tunknown mode: " << mode << std::endl;
    }

    // solver properties
    std::cout << "\nsolver properties: " << std::endl;
    std::cout << std::scientific;
    prompt("\tshow converge status?\t",showConvergeStatus);
    prompt("\tshow simplex?\t\t",showSimplex);
    prompt("\tpause?\t\t\t",pause);
    //prompt("\trelative tolerance\t",rtol);
    //prompt("\tabsolute tolerance\t",abstol);
    //prompt("\tmax iterations\t\t",iterMax);
    //prompt("\tconvergence speed\t",speed);
    std::cout << std::fixed;

    // initial solver state
    int n = 6;
    std::vector<double> initialGuess(n), lowerBound(n), upperBound(n), initialStepSize(n);

    lowerBound[0] = 0; //throttle
    lowerBound[1] = -1; // elevator
    lowerBound[2] = -90*M_PI/180; // alpha
    lowerBound[3] = -1; // aileron
    lowerBound[4] = -1; // rudder
    lowerBound[5] = -90*M_PI/180; // beta

    upperBound[0] = 1; //throttle
    upperBound[1] = 1; // elevator
    upperBound[2] = 90*M_PI/180; // alpha
    upperBound[3] = 1; // aileron
    upperBound[4] = 1; // rudder
    upperBound[5] = 90*M_PI/180; // beta

    initialStepSize[0] = 0.2; //throttle
    initialStepSize[1] = 0.1; // elevator
    initialStepSize[2] = 0.1; // alpha
    initialStepSize[3] = 0.1; // aileron
    initialStepSize[4] = 0.1; // rudder
    initialStepSize[5] = 0.1; // beta

    initialGuess[0] = 0.5; // throttle
    initialGuess[1] = 0; // elevator
    initialGuess[2] = 0; // alpha
    initialGuess[3] = 0; // aileron
    initialGuess[4] = 0; // rudder
    initialGuess[5] = 0; // beta

    // solve
    FGTrimmer trimmer(fdm, constraints);
    FGNelderMead solver(trimmer,initialGuess, lowerBound, upperBound, initialStepSize,
                        iterMax,rtol,abstol,speed,showConvergeStatus,showSimplex,pause);

	while(solver.status()==1) solver.update();		

    // output
    trimmer.printSolution(solver.getSolution()); // this also loads the solution into the fdm

    //std::cout << "\nsimulating flight to determine trim stability" << std::endl;

    //std::cout << "\nt = 5 seconds" << std::endl;
    //for (int i=0;i<5*120;i++) fdm.Run();
    //trimmer.printState();

    //std::cout << "\nt = 10 seconds" << std::endl;
    //for (int i=0;i<5*120;i++) fdm.Run();
    //trimmer.printState();

    std::cout << "\nlinearization: " << std::endl;
    FGStateSpace ss(fdm);

    // longitudinal states
    ss.x.add(new FGStateSpace::Vt);
    ss.x.add(new FGStateSpace::Alpha);
    ss.x.add(new FGStateSpace::Theta);
    ss.x.add(new FGStateSpace::Q);
    ss.x.add(new FGStateSpace::Alt);

    // lateral states
    ss.x.add(new FGStateSpace::Beta);
    ss.x.add(new FGStateSpace::Phi);
    ss.x.add(new FGStateSpace::P);
    ss.x.add(new FGStateSpace::R);
    ss.x.add(new FGStateSpace::Psi);

    // nav states
    ss.x.add(new FGStateSpace::Longitude);
    ss.x.add(new FGStateSpace::Latitude);

    // propulsion states
	if (thruster0->GetType()==FGThruster::ttPropeller)
    {
        ss.x.add(new FGStateSpace::Rpm0);
        if (variablePropPitch) ss.x.add(new FGStateSpace::PropPitch);
		int numEngines = fdm.GetPropulsion()->GetNumEngines();
		if (numEngines>1) ss.x.add(new FGStateSpace::Rpm1);
		if (numEngines>2) ss.x.add(new FGStateSpace::Rpm2);
		if (numEngines>3) ss.x.add(new FGStateSpace::Rpm3);
    }

    // input
    ss.u.add(new FGStateSpace::ThrottleCmd);
    ss.u.add(new FGStateSpace::DaCmd);
    ss.u.add(new FGStateSpace::DeCmd);
    ss.u.add(new FGStateSpace::DrCmd);

    // state feedback
    ss.y = ss.x;

    std::vector< std::vector<double> > A,B,C,D;
    std::vector<double> x0 = ss.x.get(), u0 = ss.u.get();
    std::vector<double> y0 = x0; // state feedback
    std::cout << ss << std::endl;

    ss.linearize(x0,u0,y0,A,B,C,D);
    int width=10;
    std::cout.precision(3);
    std::cout
            << std::fixed
            << std::right
            << "\nA=\n" << std::setw(width) << A
            << "\nB=\n" << std::setw(width) << B
            << "\nC=\n" << std::setw(width) << C
            << "\nD=\n" << std::setw(width) << D
            << std::endl;

    //write scicoslab file
    std::ofstream scicos(std::string(aircraft+"_lin.sce").c_str());
    scicos.precision(10);
    width=20;
    scicos
            << std::scientific
            << "x0=..\n" << std::setw(width) << x0 << ";\n"
            << "u0=..\n" << std::setw(width) << u0 << ";\n"
            << "sys = syslin('c',..\n"
            << std::setw(width) << A << ",..\n"
            << std::setw(width) << B << ",..\n"
            << std::setw(width) << C << ",..\n"
            << std::setw(width) << D << ");\n"
            << "tfm = ss2tf(sys);\n"
            << std::endl;
}

extern "C"
{
    void sci_jsbsimTrim()
    {
        trimFunction();
    }

}


// vim:ts=4:sw=4
