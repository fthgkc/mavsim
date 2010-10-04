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
#include "QOSGAdapterWidget.hpp"
#include <QFileDialog>

namespace oooark
{

MainWindow::MainWindow()
{
	setupUi(this);
	map = new ViewerQT;
	map->setCameraManipulator(new osgGA::TrackballManipulator);
}

MainWindow::~MainWindow()
{
	delete map;
}

void MainWindow::on_actionLoad_Model_activated()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Vehicle Model File"),
       ".", tr("3D Model Files (*.osg *.ac)"));

    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(fileName.toStdString());
    if (!loadedModel)
    {
        std::cout << "model not loaded" << std::endl;
    }
	map->setSceneData(loadedModel);

}

void MainWindow::on_actionLoad_Map_activated()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open OSGEarth Map File"),
       ".", tr("OSGEarth Files (*.earth)"));

    osg::ref_ptr<osg::Node> loadedMap = osgDB::readNodeFile(fileName.toStdString());
    if (!loadedMap)
    {
        std::cout << "map not loaded" << std::endl;
    }
	map->setSceneData(loadedMap);
}

} // oooark


// vim:ts=4:sw=4
