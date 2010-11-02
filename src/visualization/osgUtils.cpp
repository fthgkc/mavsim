/*
 * osgUtils.cpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * osgUtils.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * osgUtils.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "osgUtils.hpp"
#include <osg/Group>
#include <iostream>
#include <osgText/Text>
#include <osg/Geode>
#include <osg/AutoTransform>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
#include <osg/Depth>
#include <osg/PointSprite>
#include <osg/Point>
#include <osgDB/ReadFile>

namespace oooark
{

namespace visualization
{

std::vector<osg::Node*> findNamedNodes(const std::string& searchName, 
									  osg::Node* currNode)
{
   osg::Group* currGroup;
   std::vector<osg::Node*> foundNodes;
   osg::Node *foundNode;

   // check to see if we have a valid (non-NULL) node.
   // if we do have a null node, return NULL.
   if ( !currNode)
   {
	  return foundNodes;
   }

   // We have a valid node, check to see if this is the node we 
   // are looking for. If so, return the current node.
   if (currNode->getName() == searchName)
   {
	   foundNodes.push_back(currNode);
   }

   // We have a valid node, but not the one we are looking for.
   // Check to see if it has children (non-leaf node). If the node
   // has children, check each of the child nodes by recursive call.
   // If one of the recursive calls returns a non-null value we have
   // found the correct node, so return this node.
   // If we check all of the children and have not found the node,
   // return NULL
   currGroup = currNode->asGroup(); // returns NULL if not a group.
   if ( currGroup ) 
   {
	  for (unsigned int i = 0 ; i < currGroup->getNumChildren(); i ++)
	  { 
		  std::vector<osg::Node*> subNodes = findNamedNodes(searchName, currGroup->getChild(i));
		 if (subNodes.size())
			 for(int j=0;j<subNodes.size();j++) foundNodes.push_back(subNodes[j]); // found a match!
	  }
   }
   return foundNodes;
}


NodeFinder::NodeFinder(const std::string & name) :
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        myName(name), myNode(NULL)
{
}

void NodeFinder::apply(osg::Node& node)
{
    if (node.getName() == myName)
        myNode = &node;
    // traverse rest of graph
    traverse(node);
}

osg::Node* NodeFinder::getNode()
{
    return myNode.get();
}

NodeFinder::~NodeFinder()
{
}

void attachNode(const std::string & nodeName,
                osg::Node * attachNode,
                osg::Node * rootNode)
{
    NodeFinder nf(nodeName);
    nf.apply(*rootNode);
    osg::Node * node = nf.getNode();
    if (node != NULL)
    {
        osg::Group * group = node->asGroup();
        osg::Group * parent = group->getParent(0);
        parent->removeChild(group);
        parent->addChild(attachNode->asGroup());
        attachNode->asGroup()->addChild(group);
    }
    else
    {
        std::cerr << "node: " << nodeName << " not found." << std::endl;
    }
}

Label::Label(
    const osg::Vec3& position,
    const std::string& message,
    float characterSize,
    float minScale,
    float maxScale)
{
    std::string timesFont("/usr/share/fonts/truetype/ttf-liberation/LiberationSans-Regular.ttf");
    osgText::Text* text = new osgText::Text;
    text->setCharacterSize(characterSize);
    text->setText(message);
    text->setFont(timesFont);
    text->setAlignment(osgText::Text::CENTER_CENTER);

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(text);
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    osg::AutoTransform* at = new osg::AutoTransform;
    at->addChild(geode);

    at->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
    at->setAutoScaleToScreen(true);
    at->setMinimumScale(minScale);
    at->setMaximumScale(maxScale);
    at->setPosition(position);
    addChild(at);
}

Frame::Frame(
    float size,
    const std::string & xLabel,
    const std::string & yLabel,
    const std::string & zLabel,
    const osg::Vec4 & xColor,
    const osg::Vec4 & yColor,
    const osg::Vec4 & zColor)
{
    addChild(new Vector3(osg::Vec3(0,0,0),
                         osg::Vec3(size,0,0),xLabel,xColor));
    addChild(new Vector3(osg::Vec3(0,0,0),
                         osg::Vec3(0,size,0),yLabel,yColor));
    addChild(new Vector3(osg::Vec3(0,0,0),
                         osg::Vec3(0,0,size),zLabel,zColor));
}

Actuator::Actuator(
    const std::string & name,
    const osg::Vec3 & center,
    osg::Node * root) :
        myPat(new osg::PositionAttitudeTransform),
        myCenter(center)
{
    attachNode(name,myPat,root);
    myPat->setPosition(center);
    myPat->setPivotPoint(center);
}

void Actuator::setAttitude(osg::Quat quat)
{
    myPat->setAttitude(quat);
}

void Actuator::setPosition(double x, double y, double z)
{
    myPat->setPosition(osg::Vec3(x,y,z)+myCenter);
}

Vector3::Vector3(const osg::Vec3 & start,
                 const osg::Vec3 & end,
                 const std::string & name,
                 const osg::Vec4 & color) :
        myPat(new osg::PositionAttitudeTransform)
{
    osg::Geode * geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(
                           new osg::Cone(osg::Vec3(0,0,.9),.05,.1)));
    geode->addDrawable(new osg::ShapeDrawable(
                           new osg::Cylinder(osg::Vec3(0,0,.45),.01,.9)));
    osg::Material * material = new osg::Material;
    material->setDiffuse(osg::Material::FRONT,color);
    geode->getOrCreateStateSet()->setAttribute(material, osg::StateAttribute::OVERRIDE );
    myPat->addChild(new Label(osg::Vec3(0,0,1),name));
    myPat->addChild(geode);
    myPat->getOrCreateStateSet()->setMode(GL_NORMALIZE,
                                          osg::StateAttribute::ON);
    set(start,end);
    addChild(myPat);
}


void Vector3::set(const osg::Vec3 & start,
                  const osg::Vec3 & end)
{
    osg::Vec3 delta = end-start;
    double length = delta.normalize();
    osg::Vec3 zAxis(0,0,1);
    myPat->setAttitude(osg::Quat(acos(zAxis*delta),zAxis^delta));
    myPat->setPosition(start);
    myPat->setPivotPoint(start);
    myPat->setScale(osg::Vec3(1,1,1)*length);
}

Ellipsoid::Ellipsoid(osg::Vec3d radii, osg::Vec3d center, osg::Vec4 color, int vBands, int hBands) :
        myRadii(radii),
        myCenter(center),
        myVBands(vBands),
        myHBands(hBands),
        myGeom(new osg::Geometry),
        myVertices(new osg::Vec3Array),
        myNormals(new osg::Vec3Array),
        myPrimitive(new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLE_STRIP,0))
{
    osg::Geode * geode = new osg::Geode;
    osg::Vec4Array * colors = new osg::Vec4Array;

    myGeom->setVertexArray(myVertices);

    colors->push_back(color);
    myGeom->setColorArray(colors);
    myGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    myGeom->setNormalArray(myNormals);
    myGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

    myGeom->addPrimitiveSet(myPrimitive);
    geode->addDrawable(myGeom);

    addChild(geode);

    // transparency
    if (color[3] != 1)
    {
        // Enable blending, select transparent bin.
        osg::StateSet * stateSet = geode->getOrCreateStateSet();
        stateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
        stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

        // Enable depth test so that an opaque polygon will occlude a transparent one behind it.
        stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

        // Conversely, disable writing to depth buffer so that
        //a transparent polygon will allow polygons behind it to shine thru.
        //OSG renders transparent polygons after opaque ones.
        osg::Depth * depth  = new osg::Depth;
        depth->setWriteMask( false );
        stateSet->setAttributeAndModes( depth, osg::StateAttribute::ON );

        // Disable lighting
        stateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    }

    draw();
}

Ellipsoid::~Ellipsoid()
{
}

void Ellipsoid::setParam(osg::Vec3d radii, osg::Vec3d center)
{
    myRadii = radii;
    myCenter = center;
    draw();
}

void Ellipsoid::addPoint(osg::Vec3 vert)
{
    osg::Vec3 norm = vert - myCenter;
    norm.normalize();
    myNormals->push_back(norm);
    myVertices->push_back(vert);
    myPrimitive->push_back(myVertices->size()-1);
}

void Ellipsoid::draw()
{
    myNormals->clear();
    myVertices->clear();
    myPrimitive->clear();

    osg::Vec3 nadir = myCenter+osg::Vec3(0,0,myRadii.z());
    osg::Vec3 zenith = myCenter+osg::Vec3(0,0,-myRadii.z());

    double hStep = 2*M_PI/myHBands, vStep = M_PI/myVBands;
    for (double theta=0; theta<M_PI; theta+=hStep)
    {
        for (double phi=0; phi<2*M_PI; phi+=vStep)
        {
            if (phi != 0 && phi!= M_PI)
            {
                addPoint(osg::Vec3(myRadii.x()*sin(phi)*cos(theta)+myCenter.x(),
                                   myRadii.y()*sin(phi)*sin(theta)+myCenter.y(),
                                   myRadii.z()*cos(phi)+myCenter.z()));
                addPoint(osg::Vec3(myRadii.x()*sin(phi)*cos(theta+hStep)+myCenter.x(),
                                   myRadii.y()*sin(phi)*sin(theta+hStep)+myCenter.y(),
                                   myRadii.z()*cos(phi)+myCenter.z()));
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
    myGeom->dirtyDisplayList(); // force redraw
}

PointCloud::PointCloud(int pointSize) :
        myPoints(new osg::Vec3Array),
        myColors(new osg::Vec4Array),
        myGeom(new osg::Geometry),
        myDrawArrays(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, myPoints->size()))
{
    osg::Geode * geode = new osg::Geode;
    addChild(geode);
    geode->setStateSet(makeStateSet(pointSize));
    geode->addDrawable(myGeom);
    myGeom->setVertexArray(myPoints);
    myGeom->setColorArray(myColors);
    myGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    myGeom->addPrimitiveSet(myDrawArrays);
}

void PointCloud::addPoint(osg::Vec3 point, osg::Vec4 color)
{
    myPoints->push_back(point);
    myColors->push_back(color);
    updateSize();
}

void PointCloud::clear()
{
    myPoints->clear();
    myColors->clear();
    updateSize();
}

void PointCloud::updateSize()
{
    myDrawArrays->setCount(myPoints->size());
    myGeom->dirtyDisplayList();
}

osg::StateSet* PointCloud::makeStateSet(float size)
{
    osg::StateSet *set = new osg::StateSet();

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

    return set;
}

Jet::Jet() :
        model(), myLeftAileron(), myRightAileron(),
        myLeftElevator(), myRightElevator(), myRudder()
{
    model = osgDB::readNodeFile(std::string(DATADIR)+"/models/jet.ac");
    myLeftAileron.reset(new Actuator("leftAileron",osg::Vec3(-1.375,-3.330,0.485),model));
    myRightAileron.reset(new Actuator("rightAileron",osg::Vec3(-1.375,3.330,0.485),model));
    myLeftElevator.reset(new Actuator("leftElevator",osg::Vec3(-7.431,-1.934,-0.417),model));
    myRightElevator.reset(new Actuator("rightElevator",osg::Vec3(-7.431,1.934,-0.417),model));
    myRudder.reset(new Actuator("rudder",osg::Vec3(-8.279,0,-2.458),model));
    addChild(model);
}

void Jet::setEuler(double roll, double pitch, double yaw)
{
    setAttitude(osg::Quat(
                    roll,osg::Vec3(1,0,0),
                    pitch,osg::Vec3(0,1,0),
                    yaw,osg::Vec3(0,0,1)));
}

void Jet::setPositionScalars(double x, double y, double z)
{
    setPosition(osg::Vec3(x,y,z));
}

void Jet::setU(double aileron, double elevator, double rudder, double throttle)
{
    myLeftAileron->setAttitude(osg::Quat(aileron,osg::Vec3(0,1,0)));
    myRightAileron->setAttitude(osg::Quat(-aileron,osg::Vec3(0,1,0)));
    myLeftElevator->setAttitude(osg::Quat(elevator,osg::Vec3(0,1,0)));
    myRightElevator->setAttitude(osg::Quat(elevator,osg::Vec3(0,1,0)));
    myRudder->setAttitude(osg::Quat(rudder,osg::Vec3(0,0,1)));
}


} // visualization

} // oooark


// vim:ts=4:sw=4
