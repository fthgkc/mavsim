/*
 * visualization.cpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * visualization.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * visualization.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "visualization/osgUtils.hpp"
#include "visualization/Viewer.hpp"
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>

int main(int, char**)
{
    using namespace oooark::visualization;

    // construct the viewer.
    Viewer * viewer = new Viewer;

    // set the scene to render
    osg::Group * scene = new osg::Group;
    Jet * jet = new Jet();
    scene->addChild(jet);

    Vector3 * thrust = new Vector3(osg::Vec3(0,0,0),osg::Vec3(10,10,10),"thrust");
    Ellipsoid * ellipsoid = new Ellipsoid(osg::Vec3(1,2,3),osg::Vec3(10,20,10),osg::Vec4(1,0,0,.5),100,100);
    PointCloud * pointCloud = new PointCloud(4);
    scene->addChild(ellipsoid);
    scene->addChild(new Label(osg::Vec3(10,10,0),"hello"));
    scene->addChild(new Frame(15));
    scene->addChild(new Ellipsoid(osg::Vec3(3,4,5),osg::Vec3(20,20,20),osg::Vec4(0,0,1,1),100,100));
    scene->addChild(thrust);
    viewer->setSceneData(scene);
    viewer->getCamera()->setComputeNearFarMode(osgUtil::CullVisitor::DO_NOT_COMPUTE_NEAR_FAR);
    viewer->getCameraManipulator()->setHomePosition(osg::Vec3(100,100,-100),osg::Vec3(0,0,0),osg::Vec3(0,0,-1));

    // run the viewers frame loop
    viewer->run();

    double c1=0, c2= 0, c3 = 0;

    while (!viewer->done())
    {
        viewer->lock();
        ellipsoid->setParam(osg::Vec3(c1,c2,c3),osg::Vec3(10,20,10));
        pointCloud->addPoint(osg::Vec3(c1,c2,c3),osg::Vec4(c1,c2,c3,1));
        jet->setU(c1,c2,c3,c3);
        jet->setAttitude(osg::Quat(
                             c1,osg::Vec3(1,0,0),
                             c2,osg::Vec3(0,1,0),
                             c3,osg::Vec3(0,0,1)));
        jet->setPosition(osg::Vec3(c1,c2,c3));
        thrust->set(osg::Vec3(0,0,0),osg::Vec3(c1,c2,c3));
        viewer->unlock();
        usleep(1e6/20);
        c1+=.01;
        c2+=.02;
        c3+=.03;
    }
    delete viewer;
    return 0;
}
