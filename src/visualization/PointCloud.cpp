/*
 * PointCloud.cpp
 * Copyright (C) James Goppert 2009 <jgoppert@users.sourceforge.net>
 *
 * PointCloud.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PointCloud.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PointCloud.hpp"

namespace oooark
{

PointCloud::PointCloud(int pointSize) : osg::PositionAttitudeTransform(), 
        points(new osg::Vec3Array), geom(new osg::Geometry), geode(new osg::Geode), \
	   	colors(new osg::Vec4Array),
        drawArrays(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, points->size()))
{
    this->addChild(geode.get());
    geode->setStateSet(makeStateSet(pointSize));
    geode->addDrawable(geom.get());
    geom->setVertexArray(points.get());
    geom->setColorArray(colors.get());
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geom->addPrimitiveSet(drawArrays.get());
}

PointCloud::~PointCloud()
{
}

void PointCloud::setPointSize(int pointSize)
{
	geode->setStateSet(makeStateSet(pointSize));
}

void PointCloud::addPoint(osg::Vec3d point, osg::Vec4 color)
{
    points->push_back(point);
    colors->push_back(color);
    updateSize();
}

void PointCloud::clear()
{
    points->clear();
    colors->clear();
    updateSize();
}

void PointCloud::updateSize()
{
    (drawArrays.get())->setCount(points->size());
    (geom.get())->dirtyDisplayList();
}

osg::StateSet* PointCloud::makeStateSet(float size)
{
    osg::StateSet *set = new osg::StateSet();

    /// Setup cool blending
    //set->setMode(GL_BLEND, osg::StateAttribute::ON);
    //osg::BlendFunc *fn = new osg::BlendFunc();
    //fn->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::DST_ALPHA);
    //set->setAttributeAndModes(fn, osg::StateAttribute::ON);

    /// Setup the point sprites
    osg::PointSprite *sprite = new osg::PointSprite();
    set->setTextureAttributeAndModes(0, sprite, osg::StateAttribute::ON);

    /// Give some size to the points to be able to see the sprite
    osg::Point *point = new osg::Point();
    point->setSize(size);
    set->setAttribute(point);

    /// Disable depth test to avoid sort problems and Lighting
    set->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    set->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    /// The texture for the sprites
    //osg::Texture2D *tex = new osg::Texture2D();
    //tex->setImage(osgDB::readImageFile(std::string(DATADIR)+="/textures/particle.rgb"));
    //set->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);

    return set;
}

} // oooark


// vim:ts=4:sw=4
