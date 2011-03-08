/*
 * MainWindow.hpp
 * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 *
 * MainWindow.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MainWindow.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MainWindow_HPP
#define MainWindow_HPP

#include <QWidget>
#include "ui_MainWindow.h"
#include "visualization/osgUtils.hpp"
#include "math/FGNelderMead.h"
#include <iomanip>
#include <QThread>
#include "config.h"
#include <stdexcept>
#include <math/FGStateSpace.h>
#include <initialization/FGTrimmer.h>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();

signals:
	void showMsgBuffered(const QString & str);

private slots:
    void on_toolButton_enginePath_pressed();
    void on_toolButton_systemsPath_pressed();
    void on_toolButton_aircraftPath_pressed();
    void on_toolButton_aircraft_pressed();
    void on_toolButton_initScript_pressed();
    void on_pushButton_trim_pressed();
    void on_pushButton_stop_pressed();
    void on_pushButton_linearize_pressed();
	void showMsg(const QString & str);

private:
	class SolverCallback : public JSBSim::FGNelderMead::Callback
	{
	public:
		SolverCallback(MainWindow * window) : window(window)
		{
		}
		void eval(const std::vector<double> & v)
		{
			std::vector<double> data = window->trimmer->constrain(v);
			window->viewer->mutex.lock();
			window->plane->setEuler(data[0],data[1],v[5]);
				// phi, theta, beta to show orient, and side slip
			window->plane->setU(v[0],v[3],v[1],v[4]);
			window->viewer->mutex.unlock();
		}
		MainWindow * window;
	};
	SolverCallback * callback;
	class TrimThread : public QThread
	{
	public:
		TrimThread(MainWindow * window) : window(window)
		{
		}
		void run()
		{
			try
			{
				window->trim();
			}
			catch(std::exception & e)
			{
				std::cerr << "exception: " << e.what() << std::endl;
				window->showMsgBuffered(e.what());
			}
		}
		MainWindow * window;
	} trimThread;
    osg::ref_ptr<osg::Group> sceneRoot;
    void loadModel(const std::string & name);
    mavsim::visualization::Jet * plane;

	void stopSolver();
	volatile bool stopRequested;
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
	void trim();
	JSBSim::FGStateSpace * ss;
	JSBSim::FGTrimmer * trimmer;
};

#endif

// vim:ts=4:sw=4
