/*
 * Ellipsoid.hpp
 * Copyright (C) Brandon Wampler 2010 <bwampler@sourceforge.net>
 *
 * Ellipsoid.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ellipsoid.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef oooark_Ellipsoid_HPP
#define oooark_Ellipsoid_HPP

#include <iostream>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PolygonStipple>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
#include <osg/BlendFunc>
#include <osg/StateSet>
#include <osg/GL>
#include <osg/Depth>

namespace oooark
{
static const osg::Vec4d white(1,1,1,1), red(1,0,0,1), green(0,1,0,1), blue(0,0,1,1), pink(1,0,1,1);

class Ellipsoid : public osg::PositionAttitudeTransform
{
public:
    Ellipsoid(osg::Vec3d radii, osg::Vec3d center, osg::Vec4 color=red, int vBands = 10, int hBands = 10);
    void draw();
    virtual ~Ellipsoid();
    void setParam(osg::Vec3d radii, osg::Vec3d center);
	void addPoint(osg::Vec3 vec);

private:
    osg::Vec4 color;
    osg::Vec3d radii, center;
	int vBands, hBands;
	osg::ref_ptr<osg::Geode> geode;
	osg::ref_ptr<osg::Geometry> geom;
	osg::ref_ptr<osg::Vec4Array> colors;
	osg::ref_ptr<osg::Vec3Array> vertices;
	osg::ref_ptr<osg::Vec3Array> normals;
	osg::ref_ptr<osg::DrawElementsUInt> primitive;
	osg::ref_ptr<osg::StateSet> stateSet;
	osg::ref_ptr<osg::Depth> depth;
};

} // oooark

#endif

// vim:ts=4:sw=4
