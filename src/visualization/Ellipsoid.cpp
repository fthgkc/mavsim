/*
 * Ellipsoid.cpp
 * Copyright (C) Brandon Wampler 2010 <bwampler@sourceforge.net>
 *
 * Ellipsoid.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ellipsoid.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Ellipsoid.hpp"

namespace oooark
{

Ellipsoid::Ellipsoid(osg::Vec3d radii, osg::Vec3d center, osg::Vec4 color, int vBands, int hBands) : osg::PositionAttitudeTransform(),
        color(color), radii(radii), center(center), vBands(vBands), hBands(hBands),
		geode(new osg::Geode), geom(new osg::Geometry), colors(new osg::Vec4Array),
		vertices(new osg::Vec3Array), normals(new osg::Vec3Array),
		primitive(new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_STRIP,0)),
		stateSet(geode->getOrCreateStateSet()), depth(new osg::Depth)
{
	geom->setVertexArray(vertices.get()); 
	colors->push_back(color);
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	geom->setNormalArray(normals.get());
	geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
  	geom->addPrimitiveSet(primitive.get()); 
	geode->addDrawable(geom.get());
	this->addChild(geode.get());

	// transparency
	if (color[3] != 1)
	{
		// Enable blending, select transparent bin.
		//stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
		stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

		// Enable depth test so that an opaque polygon will occlude a transparent one behind it.
		//stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

		// Conversely, disable writing to depth buffer so that
		// a transparent polygon will allow polygons behind it to shine thru.
		// OSG renders transparent polygons after opaque ones.
		//depth->setWriteMask( false );
		//stateSet->setAttributeAndModes( depth, osg::StateAttribute::ON );

		// Disable lighting
		//stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	}

	draw();
}

Ellipsoid::~Ellipsoid()
{
}

void Ellipsoid::setParam(osg::Vec3d radii, osg::Vec3d center)
{
    this->radii = radii;
    this->center = center;
    draw();
}

void Ellipsoid::addPoint(osg::Vec3 vert)
{
	osg::Vec3 norm = vert - center;
	norm.normalize();
	normals->push_back(norm);
	vertices->push_back(vert);
   	primitive->push_back(vertices->size()-1);
}

void Ellipsoid::draw()
{
    //// create TRIANGLES, TRIANGLE_STRIP and TRIANGLE_FAN all in one Geometry/
		// create Geometry object to store all the vetices and lines primtive.

	normals->clear();
	vertices->clear();
	primitive->clear();

	osg::Vec3 nadir = center+osg::Vec3(0,0,radii.z());
	osg::Vec3 zenith = center+osg::Vec3(0,0,-radii.z());

	double hStep = 2*M_PI/hBands, vStep = M_PI/vBands;
	for (double theta=0; theta<M_PI; theta+=hStep)
	{
		for (double phi=0; phi<2*M_PI; phi+=vStep)
		{
			if(phi != 0 && phi!= M_PI)
			{
				addPoint(osg::Vec3(radii.x()*sin(phi)*cos(theta)+center.x(),
							radii.y()*sin(phi)*sin(theta)+center.y(),
							radii.z()*cos(phi)+center.z()));
				addPoint(osg::Vec3(radii.x()*sin(phi)*cos(theta+hStep)+center.x(),
						radii.y()*sin(phi)*sin(theta+hStep)+center.y(),
						radii.z()*cos(phi)+center.z()));
			}
			else if (phi == 0)	
			{
				addPoint(nadir);
				addPoint(nadir);
			}
			else if (phi == M_PI) 
			{
				addPoint(zenith);
				addPoint(zenith);
			}
			else std::cout<<"Error: Ellipsoid point not added"<<std::endl;
		}
	}
	addPoint(nadir); //add last point of sphere 

	geom->dirtyDisplayList();
}

} // oooark


// vim:ts=4:sw=4
