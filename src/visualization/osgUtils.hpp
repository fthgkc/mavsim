/*
 * osgUtils.hpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * osgUtils.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * osgUtils.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef osgUtils_H
#define osgUtils_H

#include <osg/NodeVisitor>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <boost/scoped_ptr.hpp>

#include <float.h>

namespace oooark
{

namespace visualization
{

class NodeFinder : public osg::NodeVisitor
{
public:
    NodeFinder(const std::string & name);
    // This method gets called for every node in the scene
    // graph. It checks each node to see if its name matches
    // the target. If so, it saves the node's address.
    virtual void apply(osg::Node& node);
    osg::Node* getNode();
    virtual ~NodeFinder();
protected:
    std::string myName;
    osg::ref_ptr<osg::Node> myNode;
};

void attachNode(const std::string & nodeName,
                osg::Node * attachNode,
                osg::Node * rootNode);

class Label : public osg::Group
{
public:
    Label(
        const osg::Vec3& position,
        const std::string& message,
        float characterSize=40,
        float minScale=0,
        float maxScale=FLT_MAX);
};

class Frame : public osg::PositionAttitudeTransform
{
public:
    Frame(
        float size=10,
        const std::string & xLabel="x",
        const std::string & yLabel="y",
        const std::string & zLabel="z",
        const osg::Vec4 & xColor = osg::Vec4(1,0,0,1),
        const osg::Vec4 & yColor = osg::Vec4(0,1,0,1),
        const osg::Vec4 & zColor = osg::Vec4(0,0,1,1));
};

class Actuator
{
public:
    Actuator(
        const std::string & name,
        const osg::Vec3 & center,
        osg::Node * root);
    void setAttitude(osg::Quat quat);
    void setPosition(double x, double y, double z);
private:
    osg::PositionAttitudeTransform * myPat;
    osg::Vec3 myCenter;
};

class Vector3 : public osg::Group
{
public:
    Vector3(const osg::Vec3 & start,
            const osg::Vec3 & end,
            const std::string & name="",
            const osg::Vec4 & color=osg::Vec4(1,0,0,1));
    void set(const osg::Vec3 & start,
             const osg::Vec3 & end);
private:
    osg::PositionAttitudeTransform * myPat;
};

class Ellipsoid : public osg::PositionAttitudeTransform
{
public:
    Ellipsoid(osg::Vec3d radii, osg::Vec3d center, osg::Vec4 color=osg::Vec4(1,0,0,1),
              int vBands = 10, int hBands = 10);
    void draw();
    virtual ~Ellipsoid();
    void setParam(osg::Vec3d radii, osg::Vec3d center);
    void addPoint(osg::Vec3 vec);

private:
    osg::Vec3d myRadii, myCenter;
    int myVBands, myHBands;
    osg::ref_ptr<osg::Geometry> myGeom;
    osg::ref_ptr<osg::Vec3Array> myVertices;
    osg::ref_ptr<osg::Vec3Array> myNormals;
    osg::ref_ptr<osg::DrawElementsUInt> myPrimitive;
};

class PointCloud : public osg::PositionAttitudeTransform
{
public:
    PointCloud(int pointSize);
    void addPoint(osg::Vec3 point, osg::Vec4 color=osg::Vec4(1,1,1,0));
    void clear();
private:
    void updateSize();
    osg::ref_ptr<osg::Vec3Array> myPoints;
    osg::ref_ptr<osg::Vec4Array> myColors;
    osg::ref_ptr<osg::Geometry> myGeom;
    osg::ref_ptr<osg::DrawArrays> myDrawArrays;
    osg::StateSet* makeStateSet(float size);
};

class Jet : public osg::PositionAttitudeTransform
{
public:
    Jet();
    void setEuler(double roll, double pitch, double yaw);
    void setPositionScalars(double x, double y, double z);
    void setU(double aileron, double elevator, double rudder, double throttle);
private:
    osg::ref_ptr<osg::Node> model;
    boost::scoped_ptr<Actuator> myLeftAileron;
    boost::scoped_ptr<Actuator> myRightAileron;
    boost::scoped_ptr<Actuator> myLeftElevator;
    boost::scoped_ptr<Actuator> myRightElevator;
    boost::scoped_ptr<Actuator> myRudder;
};

} // visualization

} // oooark

#endif

// vim:ts=4:sw=4
