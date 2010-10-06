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

#ifndef oooark_MainWindow_HPP
#define oooark_MainWindow_HPP

#include "ui_MainWindow.h"
#include "MapVehicle.hpp"
#include <osgEarthUtil/EarthManipulator>

#include <apmhsl/APM_BinComm/APM_BinComm.h>

namespace oooark
{

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
	std::vector<MapVehicle*> vehicles;
	BinComm *comm;
private slots:
	//communication 
	void updateComm();

	// control
	void on_pushButton_getGains_clicked();
	void on_pushButton_sendGains_clicked();
	void on_pushButton_returnHome_clicked();
	void on_pushButton_loiter_clicked();
	void on_pushButton_land_clicked();
	void on_pushButton_killThrottle_clicked();

	// configuration
	void on_comboBox_units_activated();
	void on_pushButton_gainsSendFile_clicked();
	void on_pushButton_gainsRequestFile_clicked();
	void on_pushButton_cameraDevice_clicked();
	void on_pushButton_vehicleFile_clicked();
	void on_pushButton_mapFile_clicked();
	void on_pushButton_telemetryPort_clicked();

	// terminal
	void on_pushButton_sendCommand_clicked();

	// guidance
	void on_pushButton_getFlightPlan_clicked();
	void on_pushButton_sendFlightPlan_clicked();
	void on_pushButton_clearFlightPlan_clicked();
	void on_pushButton_loadFlightPlan_clicked();
	
private:

	//communication
	QTimer *timer;
	BufferedAsyncSerial *serial;
	BinComm::MessageHandler handlerTable[10];
	Stream *stream;
	std::string device;
	unsigned int baud;

	// osg earth
	osgEarthUtil::EarthManipulator * earthManipulator;
	osgEarthUtil::ObjectPlacer * objectPlacer;

	// waypoint lines
	osg::Vec3dArray * wpGeo;
	osg::Vec3Array * wpXYZ;
	osg::Vec4Array * wpColors;
	osg::Geode * wpLines;
	osg::Group * wpGroup;
	osg::Geometry * wpLineGeometry;
	osg::DrawArrays * wpDrawLines;

	// vehicle track lines
	osg::Vec3dArray * vGeo;
	osg::Vec3d * vXYZ, eye, center, up;
	osg::Vec4Array * vColors;
	osg::Geode * vLines;
	osg::Group * vGroup;
	osg::Geometry * vLineGeometry;
	osg::DrawArrays * vDrawLines;
	osg::Matrixd matrix;
};

} // oooark

#endif

// vim:ts=4:sw=4
