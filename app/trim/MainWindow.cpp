/*
 * MainWindow.cpp
 * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 *
 * MainWindow.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MainWindow.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MainWindow.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <osg/Vec3d>

#include <cstdlib>
#include <fstream>
#include <models/FGAircraft.h>
#include <models/propulsion/FGEngine.h>
#include <models/propulsion/FGTurbine.h>
#include <models/propulsion/FGTurboProp.h>

MainWindow::MainWindow() : sceneRoot(new osg::Group),
	callback(new SolverCallback(this)), trimThread(this),
	ss(NULL), trimmer(NULL), fdm(NULL) 
{
    setupUi(this);
    viewer->setSceneData(sceneRoot);
    viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    viewer->getCameraManipulator()->setHomePosition(osg::Vec3d(30,30,-30),osg::Vec3d(0,0,0),osg::Vec3d(0,0,-1),false);
    viewer->getCameraManipulator()->home(0);
    sceneRoot->addChild(new mavsim::visualization::Frame(20,"N","E","D"));

	// read initial settings
	QCoreApplication::setOrganizationName("openmav");
    QCoreApplication::setOrganizationDomain("github.com/mavsim");
    QCoreApplication::setApplicationName("mavsim");

	settings = new QSettings;
	readSettings();
	writeSettings();

	// load plane
    try
    {
        plane = new mavsim::visualization::Plane;
        plane->addChild(new mavsim::visualization::Frame(15,"X","Y","Z"));
        sceneRoot->addChild(plane);
    }
    catch(const std::exception & e)
    {
		showMsg(e.what());		
    }

	// connect signals and slots
	connect(this,SIGNAL(showMsgBuffered(const QString &)),
			this,SLOT(showMsg(const QString &)),Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
	writeSettings();
    delete viewer;
}

void MainWindow::writeSettings()
{
	settings->beginGroup("mainwindow");
	settings->setValue("enginePath",lineEdit_enginePath->text());
	settings->setValue("systemsPath",lineEdit_systemsPath->text());
	settings->setValue("aircraftPath",lineEdit_aircraftPath->text());
	settings->setValue("aircraft",lineEdit_aircraft->text());
	settings->setValue("initScript",lineEdit_initScript->text());
	settings->endGroup();
}

void MainWindow::readSettings()
{
	QString root(datadir);
	settings->beginGroup("mainwindow");
	lineEdit_enginePath->setText(settings->value("enginePath",root+"/easystar/Engine").toString());
	lineEdit_systemsPath->setText(settings->value("systemsPath",root+"/easystar/Systems").toString());
	lineEdit_aircraftPath->setText(settings->value("aircraftPath",root+"/easystar").toString());
	lineEdit_aircraft->setText(settings->value("aircraft","easystar-windtunnel").toString());
	lineEdit_initScript->setText(settings->value("initScript","").toString());
	settings->endGroup();
}

void MainWindow::on_toolButton_enginePath_pressed()
{
    lineEdit_enginePath->setText(QFileDialog::getExistingDirectory(
                                     this, tr("Select Engine Path"),
                                     lineEdit_enginePath->text(),
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void MainWindow::on_toolButton_systemsPath_pressed()
{
    lineEdit_systemsPath->setText(QFileDialog::getExistingDirectory(
                                      this, tr("Select Systems Path"),
                                      lineEdit_systemsPath->text(),
                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void MainWindow::on_toolButton_aircraftPath_pressed()
{
    lineEdit_aircraftPath->setText(QFileDialog::getExistingDirectory(
                                       this, tr("Select Aircraft Path"),
                                       lineEdit_aircraftPath->text(),
                                       QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void MainWindow::on_toolButton_aircraft_pressed()
{
    QString path(QFileDialog::getExistingDirectory(
                     this, tr("Select Aircraft Directory"),
                     lineEdit_aircraftPath->text(),
                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
    if (!path.isNull())
    {
        QFileInfo pathInfo(path);
        lineEdit_aircraft->setText(pathInfo.fileName());
    }
}

void MainWindow::on_toolButton_initScript_pressed()
{
    lineEdit_initScript->setText(QFileDialog::getOpenFileName(this,
                                 tr("Select Initialization Script"),lineEdit_initScript->text(),
                                 tr("JSBSim Scripts (*.xml)")));
}

void MainWindow::on_pushButton_trim_pressed()
{
	writeSettings();
	on_pushButton_stop_pressed();
	try
	{
		trimThread.start();
	}
	catch(const std::exception & e)
	{
		QMessageBox msgBox;
		msgBox.setText(e.what());
		msgBox.exec();
	}
	catch(...)
	{
		QMessageBox msgBox;
		msgBox.setText("unknown trim exception");
		msgBox.exec();
	}
}

void MainWindow::on_pushButton_linearize_pressed()
{
	writeSettings();
	using namespace JSBSim;

	if (!ss)
	{
		showMsg("trim the aircraft first");
		return;
	}

	std::cout << "\nlinearization: " << std::endl;


	std::vector< std::vector<double> > A,B,C,D;
	std::vector<double> x0 = ss->x.get(), u0 = ss->u.get();
	std::vector<double> y0 = x0; // state feedback
	std::cout << ss << std::endl;

	ss->linearize(x0,u0,y0,A,B,C,D);
	for (int i = 0; i<A.size(); i++)
	{
		for (int j = 0; j<A[0].size(); j++)
		{
			std::cout << A[i][j];
		}
		std::cout << "\n";
	}
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

	// write scicoslab file
	std::string aircraft = lineEdit_aircraft->text().toStdString();
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

void MainWindow::on_pushButton_stop_pressed()
{
	stopSolver();
	trimThread.quit();
}

void MainWindow::on_pushButton_simulate_pressed()
{
	if (!fdm)
	{
		showMsg("trim the aircraft first");
		return;
	}

	std::cout << "\nsimulating flight to determine trim stability" << std::endl;

	std::cout << "\nt = 5 seconds" << std::endl;
	for (int i=0;i<5*120;i++)
	{
		fdm->Run();

		// TODO: add visualization
		//window->viewer->mutex.lock();
		//window->plane->setEuler(data[0],data[1],v[5]);
			//// phi, theta, beta to show orient, and side slip
		//window->plane->setU(v[0],v[3]*maxDeflection,
				//v[1]*maxDeflection,v[4]*maxDeflection);
		//window->viewer->mutex.unlock();
	}
	trimmer->printState();

	std::cout << "\nt = 10 seconds" << std::endl;
	for (int i=0;i<5*120;i++) fdm->Run();
	trimmer->printState();
}

void MainWindow::stopSolver()
{
	stopRequested = true;
}

void MainWindow::showMsg(const QString & str)
{
	QMessageBox msgBox;
	msgBox.setText(str);
	msgBox.exec();
};


void MainWindow::trim()
{
	using namespace JSBSim;

	// flight conditions
	if (fdm)
	{
		delete fdm;
		fdm = NULL;
	}
	fdm = new FGFDMExec;
	double dt = 1./atof(lineEdit_modelSimRate->text().toAscii());
	FGTrimmer::Constraints constraints;
	constraints.velocity = atof(lineEdit_velocity->text().toAscii());
	constraints.altitude= atof(lineEdit_altitude->text().toAscii());
	constraints.gamma= atof(lineEdit_gamma->text().toAscii())*M_PI/180.0;
	constraints.rollRate= atof(lineEdit_rollRate->text().toAscii());
	constraints.pitchRate= atof(lineEdit_pitchRate->text().toAscii());
	constraints.yawRate= atof(lineEdit_yawRate->text().toAscii());
	bool stabAxisRoll = checkBox_stabAxisRoll->isChecked();
	bool variablePropPitch = checkBox_variablePropPitch->isChecked();

	// paths
	std::string aircraft=lineEdit_aircraft->text().toStdString();
	std::string aircraftPath=lineEdit_aircraftPath->text().toStdString();
	std::string enginePath=lineEdit_enginePath->text().toStdString();
	std::string systemsPath=lineEdit_systemsPath->text().toStdString();
   
	// solver properties 
	bool showConvergeStatus = checkBox_showConvergence->isChecked();
	bool showSimplex = checkBox_showSimplex->isChecked();
	bool pause = checkBox_pause->isChecked();
	int debugLevel = atoi(comboBox_debugLevel->currentText().toAscii());
	double rtol = atof(lineEdit_rtol->text().toAscii());
	double abstol = atof(lineEdit_abstol->text().toAscii());
	double speed = atof(lineEdit_speed->text().toAscii());
	int iterMax = atof(lineEdit_iterMax->text().toAscii());

	// initial solver state
	int n = 6;
	std::vector<double> initialGuess(n), lowerBound(n), upperBound(n), initialStepSize(n);

	lowerBound[0] = atof(lineEdit_throttleMin->text().toAscii())/100.0;
	lowerBound[1] = atof(lineEdit_elevatorMin->text().toAscii())/100.0;
	lowerBound[2] = atof(lineEdit_alphaMin->text().toAscii())*M_PI/180.0;
	lowerBound[3] = atof(lineEdit_aileronMin->text().toAscii())/100.0;
	lowerBound[4] = atof(lineEdit_rudderMin->text().toAscii())/100.0;
	lowerBound[5] = atof(lineEdit_betaMin->text().toAscii())*M_PI/180.0;

	upperBound[0] = atof(lineEdit_throttleMax->text().toAscii())/100.0; 
	upperBound[1] = atof(lineEdit_elevatorMax->text().toAscii())/100.0; 
	upperBound[2] = atof(lineEdit_alphaMax->text().toAscii())*M_PI/180.0; 
	upperBound[3] = atof(lineEdit_aileronMax->text().toAscii())/100.0; 
	upperBound[4] = atof(lineEdit_rudderMax->text().toAscii())/100.0; 
	upperBound[5] = atof(lineEdit_betaMax->text().toAscii())*M_PI/180.0; 

	initialGuess[0] = atof(lineEdit_throttleGuess->text().toAscii())/100.0; 
	initialGuess[1] = atof(lineEdit_elevatorGuess->text().toAscii())/100.0; 
	initialGuess[2] = atof(lineEdit_alphaGuess->text().toAscii())*M_PI/180.0; 
	initialGuess[3] = atof(lineEdit_aileronGuess->text().toAscii())/100.0; 
	initialGuess[4] = atof(lineEdit_rudderGuess->text().toAscii())/100.0; 
	initialGuess[5] = atof(lineEdit_betaGuess->text().toAscii())*M_PI/180.0; 

	initialStepSize[0] = atof(lineEdit_throttleInitialStepSize->text().toAscii())/100.0; 
	initialStepSize[1] = atof(lineEdit_elevatorInitialStepSize->text().toAscii())/100.0; 
	initialStepSize[2] = atof(lineEdit_alphaInitialStepSize->text().toAscii())*M_PI/180.0; 
	initialStepSize[3] = atof(lineEdit_aileronInitialStepSize->text().toAscii())/100.0; 
	initialStepSize[4] = atof(lineEdit_rudderInitialStepSize->text().toAscii())/100.0; 
	initialStepSize[5] = atof(lineEdit_betaInitialStepSize->text().toAscii())*M_PI/180.0; 

	// loading
	std::cout << "\n==============================================\n";
	std::cout << "\tJSBSim Trimming Utility\n";
	std::cout << "==============================================\n" << std::endl;

	fdm->Setdt(dt);

	if (!fdm->LoadModel(aircraftPath,enginePath,systemsPath,aircraft,false))
	{
		showMsgBuffered("model paths incorrect");
		return;
	}
	std::string aircraftName = fdm->GetAircraft()->GetAircraftName();
	std::cout << "\tsuccessfully loaded: " <<  aircraftName << std::endl;

	// Turn on propulsion system
	fdm->GetPropulsion()->InitRunning(-1);

	// get propulsion pointer to determine type/ etc.
	FGEngine * engine0 = fdm->GetPropulsion()->GetEngine(0);
	FGThruster * thruster0 = engine0->GetThruster();

	
	// solve
	if (trimmer) delete trimmer;
	trimmer = new FGTrimmer(*fdm,constraints);
	FGNelderMead solver(*trimmer,initialGuess, lowerBound, upperBound, initialStepSize,
						iterMax,rtol,abstol,speed,showConvergeStatus,showSimplex,pause,callback);
	stopRequested = false;
	while(!stopRequested && solver.status()==1) solver.update();

	if (stopRequested) return;
	else if (solver.status()!=0)
	{
		showMsgBuffered("solver failed");
		return;
	}

	showMsgBuffered("solver converged");

	// output
	trimmer->printSolution(solver.getSolution()); // this also loads the solution into the fdm

	//std::cout << "\nsimulating flight to determine trim stability" << std::endl;

	//std::cout << "\nt = 5 seconds" << std::endl;
	//for (int i=0;i<5*120;i++) fdm.Run();
	//trimmer->printState();

	//std::cout << "\nt = 10 seconds" << std::endl;
	//for (int i=0;i<5*120;i++) fdm.Run();
	//trimmer->printState();

	ss = new FGStateSpace(*fdm);

	// longitudinal states
	ss->x.add(new FGStateSpace::Vt);
	ss->x.add(new FGStateSpace::Alpha);
	ss->x.add(new FGStateSpace::Theta);
	ss->x.add(new FGStateSpace::Q);
	ss->x.add(new FGStateSpace::Alt);

	// lateral states
	ss->x.add(new FGStateSpace::Beta);
	ss->x.add(new FGStateSpace::Phi);
	ss->x.add(new FGStateSpace::P);
	ss->x.add(new FGStateSpace::R);
	ss->x.add(new FGStateSpace::Psi);

	// nav states
	ss->x.add(new FGStateSpace::Longitude);
	ss->x.add(new FGStateSpace::Latitude);

	// propulsion states
	if (thruster0->GetType()==FGThruster::ttPropeller)
	{
		ss->x.add(new FGStateSpace::Rpm);
		if (variablePropPitch) ss->x.add(new FGStateSpace::PropPitch);
	}

	// input
	ss->u.add(new FGStateSpace::ThrottleCmd);
	ss->u.add(new FGStateSpace::DaCmd);
	ss->u.add(new FGStateSpace::DeCmd);
	ss->u.add(new FGStateSpace::DrCmd);

	// state feedback
	ss->y = ss->x;
}

// vim:ts=4:sw=4
