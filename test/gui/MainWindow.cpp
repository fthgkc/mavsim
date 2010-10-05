/*
 * MainWindow.cpp * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 * * MainWindow.cpp is free software: you can redistribute it and/or modify it * under the terms of the GNU General Public License as published by the
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
#include <osgEarthUtil/EarthManipulator>

namespace oooark
{

MainWindow::MainWindow() : 
	earthManipulator(new osgEarthUtil::EarthManipulator), objectPlacer()
{
	setupUi(this);
	map->setCameraManipulator(new osgEarthUtil::EarthManipulator);
	map->setSceneData(new osg::Group);
	map->show();
}

MainWindow::~MainWindow()
{
	delete map;
}

// control

void MainWindow::on_pushButton_getGains_clicked()
{
	std::cout << "get gains" << std::endl;
}

void MainWindow::on_pushButton_sendGains_clicked()
{
	std::cout << "send gains" << std::endl;
}

void MainWindow::on_pushButton_returnHome_clicked()
{
	std::cout << "return home" << std::endl;
}

void MainWindow::on_pushButton_loiter_clicked()
{
	std::cout << "loiter" << std::endl;
}

void MainWindow::on_pushButton_land_clicked()
{
	std::cout << "land" << std::endl;
}

void MainWindow::on_pushButton_killThrottle_clicked()
{
	std::cout << "kill throttle" << std::endl;
}

// configuration
void MainWindow::on_comboBox_units_activated()
{
	std::cout << "units" << std::endl;
}

void MainWindow::on_pushButton_gainsSendFile_clicked()
{
	std::cout << "send gains file" << std::endl;
}

void MainWindow::on_pushButton_gainsRequestFile_clicked()
{
	std::cout << "get gains file" << std::endl;
}

void MainWindow::on_pushButton_cameraDevice_clicked()
{
	std::cout << "select camera device" << std::endl;
}

void MainWindow::on_pushButton_vehicleFile_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Vehicle Model File"),
       ".", tr("3D Model Files (*.osg *.ac)"));

    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(fileName.toStdString());
    if (!loadedModel)
    {
        std::cout << "model not loaded" << std::endl;
    }
	map->getSceneData()->asGroup()->addChild(loadedModel);
}

void MainWindow::on_pushButton_mapFile_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open OSGEarth Map File"),
       ".", tr("OSGEarth Files (*.earth)"));

    osg::ref_ptr<osg::Node> loadedMap = osgDB::readNodeFile(fileName.toStdString());
    if (!loadedMap)
    {
        std::cout << "map not loaded" << std::endl;
    }
	if(loadedMap.valid())
	{
		if (map->getSceneData()) map->setSceneData(NULL);
		map->setSceneData(loadedMap);
		objectPlacer = new osgEarthUtil::ObjectPlacer(loadedMap);
	}
	else
	{
		std::cout<<"map not valid"<<std::endl;
	}

}

void MainWindow::on_pushButton_telemetryPort_clicked()
{
	std::cout << "select telemetry port" << std::endl;
}

// terminal
void MainWindow::on_pushButton_sendCommand_clicked()
{
	std::cout << "send command" << std::endl;
}

// guidance
void MainWindow::on_pushButton_getFlightPlan_clicked()
{
	std::cout << "get flight plan" << std::endl;
}

void MainWindow::on_pushButton_sendFlightPlan_clicked()
{
	std::cout << "send flight plan" << std::endl;
}

void MainWindow::on_pushButton_clearFlightPlan_clicked()
{
	std::cout << "clear flight plan" << std::endl;
}

void MainWindow::on_pushButton_loadFlightPlan_clicked()
{
	std::cout << "load flight plan" << std::endl;
}

// map

void MainWindow::on_addWaypoint_clicked()
{
      //osgUtil::LineSegmentIntersector::Intersections intersections;
	//float x = 0, y = 0, height = 0; // TODO: set to values from qt mouse event
	//if (map->computeIntersections(x, height - y , intersections))
	//{
		////std::cout<<"contains intersections"<<std::endl;
		//osg::Vec3d xyz;
		//earthManipulator->screenToWorld(x, height - y, map->getCamera()->getView(), xyz);
		//osg::EllipsoidModel ellipsoid;
		//double lat, lon, alt, wpDispAlt;

		//earthManipulator->getSRS()->getEllipsoid()->convertXYZToLatLongHeight(xyz.x(), xyz.y(), xyz.z(), lat, lon, alt);
		//if (alt<0) alt = 0; //if intercept calculate from clicking screen is calculated below 0, reset to 0;
		//wpDispAlt = alt + 10;
		//earthManipulator->getSRS()->getEllipsoid()->convertLatLongHeightToXYZ( lat, lon, wpDispAlt, xyz.x(), xyz.y(), xyz.z());

		////Add wp marker
		//osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		//osg::ShapeDrawable * drawableSphere = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(xyz.x(), xyz.y(), xyz.z()), 5));
		//geode->addDrawable(drawableSphere);
		//wpGroup->addChild(geode);

		////Add wpLines
		//wpXYZ->push_back(xyz);
		////wpGeo->push_back(osg::Vec3d(lat, lon, alt));
		//wpColors->push_back(osg::Vec4(1,0,0,1));
		//wpDrawLines->setCount(wpXYZ->size());
		////vehicle->addWp(lat, lon, alt);
	//}
}


} // oooark


// vim:ts=4:sw=4
