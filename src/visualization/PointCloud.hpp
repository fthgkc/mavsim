/*
 * PointCloud.hpp
 * Copyright (C) James Goppert 2009 <jgoppert@users.sourceforge.net>
 *
 * PointCloud.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PointCloud.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef oooark_PointCloud_HPP
#define oooark_PointCloud_HPP

#include <iostream>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/PointSprite>
#include <osg/Point>
#include <osg/StateAttribute>
#include <osg/BlendFunc>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

namespace oooark
{
static const osg::Vec4d white(1,1,1,1), red(1,0,0,1), green(0,1,0,1), blue(0,0,1,1), pink(1,0,1,1);

class PointCloud : public osg::PositionAttitudeTransform
{
public:
    PointCloud(int pointSize=1);
    virtual ~PointCloud();
    void setPointSize(int pointSize);
    void addPoint(osg::Vec3d point, osg::Vec4 color=green);
    void clear();
private:
    void updateSize();
    osg::ref_ptr<osg::Vec3Array> points;
    osg::ref_ptr<osg::Vec4Array> colors;
    osg::ref_ptr<osg::Geode> geode;
    osg::ref_ptr<osg::Geometry> geom;
    osg::ref_ptr<osg::DrawArrays> drawArrays;
    osg::StateSet* makeStateSet(float size);
};

} // oooark

#endif

// vim:ts=4:sw=4
