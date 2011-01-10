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
#include "JSBSim/math/FGNelderMead.h"
#include <iomanip>
#include <boost/thread.hpp>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();

private slots:
    void on_toolButton_enginePath_pressed();
    void on_toolButton_systemsPath_pressed();
    void on_toolButton_aircraftPath_pressed();
    void on_toolButton_aircraft_pressed();
    void on_toolButton_initScript_pressed();
    void on_pushButton_trim_pressed();
    void on_pushButton_stop_pressed();

private:
	class SolverCallback : public JSBSim::FGNelderMead::Callback
	{
	public:
		SolverCallback(MainWindow * window) : window(window)
		{
		}
		void eval(const std::vector<double> & v)
		{
			window->viewer->mutex.lock();
  			window->plane->setEuler(0,v[2],v[5]);
    		window->plane->setU(v[3],v[1],v[4],v[0]);
			window->viewer->mutex.unlock();
		}
		MainWindow * window;
	};
	SolverCallback * callback;
	boost::thread * trimThread;
    osg::ref_ptr<osg::Group> sceneRoot;
    void loadModel(const std::string & name);
    oooark::visualization::Jet * plane;
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
};

#endif

// vim:ts=4:sw=4
