/*
 * MapVehicle.hpp
 * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 *
 * MapVehicle.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MapVehicle.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef oooark_MapVehicle_HPP
#define oooark_MapVehicle_HPP

#include <osgEarthUtil/ObjectPlacer>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <string>
//#include "communication/ApmProtocol.hpp" 
#include "visualization/PointCloud.hpp"
#include <sys/time.h>

namespace oooark
{

class MapVehicle : public osg::Group
{
private:
	oooark::PointCloud* traceCloud;
    osgEarthUtil::ObjectPlacer* placer;
    osg::Node * model;
    osg::PositionAttitudeTransform * paTransform;
    osg::MatrixTransform *matrixTransform, *traceMatrixTransform;
    osg::Matrixd matrix, traceMatrix;
    osg::Cylinder * cylinder;
    osg::Vec3d vXYZ;
	std::string geoLabel;
    double lat, lon, alt, roll, pitch, yaw, groundSpeed, groundCourse, timeOfWeek;
    std::vector<int32_t> latVec, lonVec, altVec;

public:
    bool trace;

    MapVehicle(osgEarthUtil::ObjectPlacer* placer, std::string modelFile, std::string device,\
            const long int baud, std::string * geoLabel);
    virtual ~MapVehicle();
	void clearTrace();
    void addWp(double lat, double lon, double alt);
    void clearWpList();
    void sendWpList();
	void printNavData();
    void update();
    osg::Node * getTetherNode();
    osg::Matrixd getMatrix();
    osg::Vec3d getGeo();
    osg::Vec3d getFocalPoint();
};

} // oooark namespace

#endif

// vim:ts=4:sw=4
