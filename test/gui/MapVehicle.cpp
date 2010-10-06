/*
 * MapVehicle.cpp
 * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 *
 * MapVehicle.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MapVehicle.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MapVehicle.hpp"

namespace oooark
{

MapVehicle::MapVehicle(osgEarthUtil::ObjectPlacer* placer, osg::Node * loadedModel) : Group(),	\
		trace(false), placer(placer), model(loadedModel),  \
		paTransform(new osg::PositionAttitudeTransform), \
		matrixTransform(new osg::MatrixTransform), traceMatrixTransform(new osg::MatrixTransform), \
		matrix(), traceCloud(new oooark::PointCloud(2)), \
		lat(40.430896), lon(-86.914602), altMsl(100), roll(0), pitch(0), yaw(0)
{
	matrixTransform->addChild(paTransform);
	paTransform->setScale(osg::Vec3d(1,1,1));
	paTransform->addChild(model);
	this->addChild(matrixTransform);
	this->addChild(traceCloud);

	//Marker cylinder from plane to ground
	osg::Geode * marker = new osg::Geode;
	cylinder = new osg::Cylinder(osg::Vec3(0, 0, 0), 0.3, 100);
	osg::ShapeDrawable *drawableCylinder = new osg::ShapeDrawable;
	drawableCylinder = new osg::ShapeDrawable(cylinder);
	drawableCylinder->setUseDisplayList(false);
	drawableCylinder->setColor(osg::Vec4d(1.0,0,0,1.0));
	marker->addDrawable(drawableCylinder);

}

MapVehicle::~MapVehicle()
{
}

void MapVehicle::clearTrace()
{
	traceCloud->clear();
}

void MapVehicle::addWp(double lat, double lon, double alt)
{
	latVec.push_back(lat*180/M_PI*1e7);
	lonVec.push_back(lon*180/M_PI*1e7);
	altVec.push_back(alt*180/M_PI*1e2);

}
void MapVehicle::clearWpList()
{
	latVec.clear();
	lonVec.clear();
	altVec.clear();
}
void MapVehicle::sendWpList()
{
	//std::vector<char> message;
	//int16_uint8 messageType;
	//int32_uint8 lat, lon, alt;
	//message.push_back(messageType.asUint8[0]);
	//message.push_back(messageType.asUint8[1]);
	//for (int i = 0; i<latVec.size(); i++)
	//{
		//lat.asInt32 = latVec.at(i);
		//lon.asInt32 = lonVec.at(i);
		//alt.asInt32 = altVec.at(i);
		//for (int j = 0; j<4; j++) message.push_back(lat.asUint8[j]);
		//for (int j = 0; j<4; j++) message.push_back(lon.asUint8[j]);
		//for (int j = 0; j<4; j++) message.push_back(alt.asUint8[j]);
	//}

	////comm.send(message);

	//for (int i = 0; i < latVec.size(); i++)
	//{
		//std::cout << "lat: " << latVec.at(i) << "  lon: " << lonVec.at(i) << "  alt: " << altVec.at(i) << std::endl;
	//}
}
void MapVehicle::printNavData()
{
	std::cout << "lat: " << lat;
	std::cout << "  lon: " << lon;
	std::cout << "  alt: " << altMsl << std::endl;
	std::cout << "roll: " << roll;
	std::cout << "  pitch: " << pitch;
	std::cout << "  yaw: " << yaw << std::endl;
	std::cout << std::endl;
}

void MapVehicle::update()
{
	printNavData();

	// correct for coordinate frame of cessna model and put into
	// a quaternion
	yaw = -yaw;
	double tmp = pitch;
	pitch = roll;
	roll = tmp;
	double c1, c2, c3, s1, s2, s3;
	c1 = cos(roll/2.);
	c2 = cos(pitch/2.);
	c3 = cos(yaw/2.);
	s1 = sin(roll/2);
	s2 = sin(pitch/2.);
	s3 = sin(yaw/2.);
	//set vehicle attitude
	osg::Quat quat(s1*c2*c3-c1*s2*s3, c1*s2*c3+s1*c2*s3, c1*c2*s3+s1*s2*c3, c1*c2*c3+s1*s2*s3 );
	paTransform->setAttitude(quat);
	
	//set vehicle position on map
	bool status = placer->createPlacerMatrix(lat, lon, altMsl, matrix);
	if (status) 
	{
		matrixTransform->setMatrix(matrix);
		//set cylinder that runs from plane to the ground
		cylinder->setHeight(altMsl);
		cylinder->setCenter(osg::Vec3(0,0,altMsl/2));
	}
	else 
	{
		std::cout<<"Placer Matrix Error"<<std::endl;
	}

	if (trace)
	{
		//place marker at altitude
		traceCloud->addPoint(matrixTransform->getMatrix().getTrans());
		
		//place marker on the ground
		bool status = placer->createPlacerMatrix(lat, lon, 0, traceMatrix);
		if (status) 
		{
			traceMatrixTransform->setMatrix(traceMatrix);
			traceCloud->addPoint(traceMatrixTransform->getMatrix().getTrans(), red);
		}
		else 
		{
			std::cout<<"Placer Matrix Error"<<std::endl;
		}
	}
}
osg::Node * MapVehicle::getTetherNode()
{
	return matrixTransform;
}
osg::Matrixd MapVehicle::getMatrix()
{
	return matrixTransform->getInverseMatrix();
}
osg::Vec3d MapVehicle::getGeo()
{
	return osg::Vec3d(lat, lon, altMsl);
}
osg::Vec3d MapVehicle::getFocalPoint()
{
	return osg::Vec3d(lon, lat, altMsl);
}


} // oooark


// vim:ts=4:sw=4
