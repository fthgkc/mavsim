/***************************************************************************
 *   Copyright (C) 2003 by Rick L. Vinyard, Jr.                            *
 *   rvinyard@cs.nmsu.edu                                                  *
 *                                                                         *
 *   This file is part of the osgGtk library.                              *
 *                                                                         *
 *   The osgGtk library is free software; you can redistribute it and/or   *
 *   modify it under the terms of the GNU General Public License           *
 *   version 3 as published by the Free Software Foundation.               *
 *                                                                         *
 *   The osgGtk library is distributed in the hope that it will be         *
 *   useful, but WITHOUT ANY WARRANTY; without even the implied warranty   *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
 *   General Public License for more details.                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this software. If not see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

// Of course, we need the gtkmm headers
#include <gtkmm.h>
#include <gtkmm/messagedialog.h>

// We'll need this to load the command line argument
// as well as the handling the file load button
#include <osgDB/ReadFile>

#include <osgEarthUtil/ObjectPlacer>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgEarthUtil/EarthManipulator>
#include <osg/ShapeDrawable>

// And we'll need this for the Gtkmm OSG viewer
#include <osgGtkmm/ViewerGtkmm.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <string>
#include "communication/ApmProtocol.hpp" 
#include "visualization/PointCloud.hpp"
#include <sys/time.h>
namespace oooark
{

bool fileExists(const char * filename)
{
    if (FILE * file = fopen(filename, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}     

//union int16_uint8
//{
	//int16_t asInt16;
	//uint8_t asUint8[2];
//};

//union int32_uint8
//{
	//int32_t asInt32;
	//uint8_t asUint8[4];
//};

class MapVehicle : public osg::Group
{
private:
	timeval currentTime, lastTime;
	double diff, updateFreq;
	oooark::PointCloud* traceCloud;
    osgEarthUtil::ObjectPlacer* placer;
    osg::Node * model;
    ApmProtocol comm;
    osg::PositionAttitudeTransform * paTransform;
    osg::MatrixTransform *matrixTransform, *traceMatrixTransform;
    osg::Matrixd matrix, traceMatrix;
    osg::Cylinder * cylinder;
    boost::thread * thread;
    osg::Vec3d vXYZ;
    Gtk::Label * geoLabel;
    double lat, lon, alt, roll, pitch, yaw, groundSpeed, groundCourse, timeOfWeek;
    std::vector<int32_t> latVec, lonVec, altVec;

public:
    bool trace;

    MapVehicle(osgEarthUtil::ObjectPlacer* placer, std::string modelFile, std::string device,\
            const long int baud, Gtk::Label * geoLabel) : Group(),	\
            trace(false), placer(placer), model(osgDB::readNodeFile(modelFile)), comm(device, baud),  \
			paTransform(new osg::PositionAttitudeTransform), \
            matrixTransform(new osg::MatrixTransform), traceMatrixTransform(new osg::MatrixTransform), \
            matrix(), traceCloud(new oooark::PointCloud(2)), \
            thread(), lat(40.430896), lon(-86.914602), alt(100), roll(0), pitch(0), yaw(0)
    {
		gettimeofday(&currentTime, NULL);
		gettimeofday(&lastTime, NULL);

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

		updateFreq=12;
        thread = new boost::thread( boost::bind( &MapVehicle::update, this ) );

    }

    virtual ~MapVehicle()
    {
        if (thread) thread->join();
        delete thread;
    }

	void clearTrace()
	{
		traceCloud->clear();
	}

    void addWp(double lat, double lon, double alt)
    {
        latVec.push_back(lat*180/M_PI*1e7);
        lonVec.push_back(lon*180/M_PI*1e7);
        altVec.push_back(alt*180/M_PI*1e2);

    }
    void clearWpList()
    {
        latVec.clear();
        lonVec.clear();
        altVec.clear();
    }
    void sendWpList()
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
	void printNavData()
	{
		std::cout << "lat: " << lat;
		std::cout << "  lon: " << lon;
		std::cout << "  alt: " << alt << std::endl;
		std::cout << "roll: " << roll*180.0/M_PI;
		std::cout << "  pitch: " << pitch*180.0/M_PI;
		std::cout << "  yaw: " << yaw*180.0/M_PI << std::endl;
		std::cout << std::endl;
	}

    void update()
    {
        while (1)
        {
			gettimeofday(&currentTime, NULL);
			diff = (currentTime.tv_sec-lastTime.tv_sec) + (currentTime.tv_usec-lastTime.tv_usec)/1e6;
			if(diff>(1.0/updateFreq))
			{
				comm.update();
				if(comm.newNavData)
				{
					lastTime = currentTime;

					comm.getNavData(roll, pitch, yaw,lat, lon, alt, groundSpeed, groundCourse, timeOfWeek);
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
					bool status = placer->createPlacerMatrix(lat, lon, alt, matrix);
					if (status) matrixTransform->setMatrix(matrix);
					else {std::cout<<"Placer Matrix Error"<<std::endl; continue;}

					//set cylinder that runs from plane to the ground
					cylinder->setHeight(alt);
					cylinder->setCenter(osg::Vec3(0,0,alt/2));

					//Set Info Label
					//char stringBuffer [100];
					//sprintf (stringBuffer, "Lat:  %10.8Lf\n Lon: %10.8Lf\n Alt:  %10.8Lf", lat, lon, alt);
					//geoLabel->set_label(stringBuffer);

					if (trace)
					{
						//place marker at altitude
						traceCloud->addPoint(matrixTransform->getMatrix().getTrans());
						
						//place marker on the ground
						bool status = placer->createPlacerMatrix(lat, lon, 0, traceMatrix);
						if (status) traceMatrixTransform->setMatrix(traceMatrix);
						else {std::cout<<"Placer Matrix Error"<<std::endl; continue;}
						traceCloud->addPoint(traceMatrixTransform->getMatrix().getTrans(), red);
					}
				}
			}
			usleep((1.0/(updateFreq+1))*1e6);
        }
    }
    osg::Node * getMatrixTransformNode()
    {
        return matrixTransform;
    }
    osg::Matrixd getMatrix()
    {
        return matrixTransform->getInverseMatrix();
    }
    osg::Vec3d getGeo()
    {
        return osg::Vec3d(lat, lon, alt);
    }
    osg::Vec3d getFocalPoint()
    {
        return osg::Vec3d(lon, lat, alt);
    }


};

} // oooark namespace

using namespace oooark;
// Callback to handle mouse button events
// This is what will look for right-clicks and pop up the menu
bool on_button_event(GdkEventButton* event);

// Callback that loads a selected scene as the root node of the scenegraph
void on_open_file_clicked();

// Callback that loads a seleddcted scene as the root node of the scenegraph
void on_open_vehicle_clicked();
void on_addWp_clicked(GdkEventButton & event);
void on_center_vehicle_clicked();
void on_follow_vehicle_clicked();
void on_stop_follow_vehicle_clicked();
void on_start_trace_clicked();
void on_reset_trace_clicked();
void on_reset_wp_clicked();
void on_send_wp_clicked();

osgViewer::ViewerGtkmm* viewer = NULL;
osgViewer::GraphicsWindowGtkmm* gw = NULL;
MapVehicle * vehicle;
//Wp Lines
osg::Vec3dArray * wpGeo;
osg::Vec3Array * wpXYZ;
osg::Vec4Array * wpColors;
osg::Geode * wpLines;
osg::Group * wpGroup;
osg::Geometry * wpLineGeometry;
osg::DrawArrays * wpDrawLines;

//Vehicle Track Lines
osg::Vec3dArray * vGeo;
osg::Vec3d * vXYZ, eye, center, up;
osg::Vec4Array * vColors;
osg::Geode * vLines;
osg::Group * vGroup;
osg::Geometry * vLineGeometry;
osg::DrawArrays * vDrawLines;
osg::Matrixd matrix;


Gtk::ToggleButton * start_stop_button;
Gtk::Label * geo_label;

bool modelLoaded = false;

osgEarthUtil::ObjectPlacer* objectPlacer;
osgEarthUtil::EarthManipulator * earthManipulator = new osgEarthUtil::EarthManipulator;

int main( int argc, char** argv )
{


    // We'll set a basic size of 800x600, but the user can resize larger if desired (but not smaller)
    int width=800, height=600;

    // This is some gtkmm housekeeping... we need the main loop and we also need to initialize the OpenGL system
    Gtk::Main gtkmm_main( argc, argv );
    Gtk::GL::init( argc, argv );

    // Now, let's create our viewer using ViewerGtkmm so we can use the gtkmm convenience method later
    //
    // We'll also set desired frame rate of 30 fps and load the first command line
    // argument as the scene

	earthManipulator->getSettings()->setMinMaxPitch(-90,-30);

    viewer = new osgViewer::ViewerGtkmm();
    viewer->setCameraManipulator(earthManipulator);
    viewer->set_fps(10);

    // Now we'll setup the viewer in a gtkmm window using the setup_viewer_in_gtkmm_window()
    // convenience method
    //
    // The return type is a Gtkmm widget (subclassed from Gtk::DrawingArea) that we can
    // use anywhere we want
    gw = viewer->setup_viewer_in_gtkmm_window(width, height);

    // All Gtk apps need a toplevel window
    //
    // We'll also set the title, add our widget created by the viewer and make sure it's visible
    Gtk::Window window(Gtk::WINDOW_TOPLEVEL);
    window.set_title("oooark Ground Station");

    // Almost all of this is just gtkmm code... creating the various
    // layouts and other UI widgets
    Gtk::VBox main_vbox;
    Gtk::HBox bottom_hbox;
    Gtk::VBox button_vbox;
    Gtk::VBox button_vbox2;
    Gtk::VBox info_vbox;

    //Gtk::Label help_text(HELP_TEXT);
    Gtk::Button file_button("Load Map");
    Gtk::Button close_button("Close");
    Gtk::Button vehicle_button("Load Vehicle");
    start_stop_button = Gtk::manage(new Gtk::ToggleButton("Start Trace"));
    geo_label = Gtk::manage(new Gtk::Label("Vehicle Information"));
    Gtk::Button reset_button("Reset Trace");
    Gtk::Button reset_wp_button("Reset Waypoints");
    Gtk::Button send_wp_button("Send Waypoint List");

    window.add(main_vbox);
    main_vbox.pack_start(*gw);
    main_vbox.pack_start(bottom_hbox, Gtk::PACK_SHRINK);
    bottom_hbox.pack_start(button_vbox, Gtk::PACK_SHRINK);
    bottom_hbox.pack_start(button_vbox2, Gtk::PACK_SHRINK);
    bottom_hbox.pack_start(info_vbox);
    info_vbox.pack_start(*geo_label);

    button_vbox.pack_start(file_button, Gtk::PACK_SHRINK);
    button_vbox.pack_start(vehicle_button, Gtk::PACK_SHRINK);
    button_vbox.pack_start(close_button, Gtk::PACK_SHRINK);
    button_vbox2.pack_start(*start_stop_button, Gtk::PACK_SHRINK);
    button_vbox2.pack_start(reset_button, Gtk::PACK_SHRINK);
    button_vbox2.pack_start(reset_wp_button, Gtk::PACK_SHRINK);
    button_vbox2.pack_start(send_wp_button, Gtk::PACK_SHRINK);

    window.show_all();

    // We need to handle the case where the button is pressed with the
    // shift or control key. Otherwise the button press will be passed on to
    // the gtk handler.
    gw->signal_button_press_event().connect(&on_button_event);
    gw->signal_button_release_event().connect(&on_button_event);

    // Let's connect the other Gtk buttons to their callbacks
    file_button.signal_clicked().connect(&on_open_file_clicked);
    vehicle_button.signal_clicked().connect(&on_open_vehicle_clicked);
    close_button.signal_clicked().connect(&Gtk::Main::quit);
    start_stop_button->signal_toggled().connect(&on_start_trace_clicked);
    reset_button.signal_clicked().connect(&on_reset_trace_clicked);
    reset_wp_button.signal_clicked().connect(&on_reset_wp_clicked);
    send_wp_button.signal_clicked().connect(&on_send_wp_clicked);
    //Setup wp lines
    wpGeo = new osg::Vec3dArray;
    wpXYZ = new osg::Vec3Array;
    wpColors = new osg::Vec4Array;
    wpLines = new osg::Geode;
    wpGroup = new osg::Group;
    wpLineGeometry = new osg::Geometry;
    wpDrawLines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 0);
    wpLines->addDrawable(wpLineGeometry);
    wpLineGeometry->setVertexArray(wpXYZ);
    wpLineGeometry->setColorArray(wpColors);
    wpLineGeometry->addPrimitiveSet(wpDrawLines);
    wpLineGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    wpLineGeometry->setUseDisplayList(false);

    //Setup vehicle track lines
    //vGeo = new osg::Vec3dArray;
    //vXYZ = new osg::Vec3Array;
    //vColors = new osg::Vec4Array;
    //vLines = new osg::Geode;
    //vGroup = new osg::Group;
    //vLineGeometry = new osg::Geometry;
    //vDrawLines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 0);
    //vLines->addDrawable(vLineGeometry);
    //vLineGeometry->setVertexArray(vXYZ);
    //vLineGeometry->setColorArray(vColors);
    //vLineGeometry->addPrimitiveSet(vDrawLines);
    //vLineGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    //vLineGeometry->setUseDisplayList(false);

    // This is boilerplate for all gtkmm apps... start the gtkmm main loop
    Gtk::Main::run(window);
    // If we got here, gtkmm exited it's main loop, so we'll exit cleanly
    return 0;
}

// This function is called every time there is a button press or button
// release event that occurs in the OSG graphics window
//
// Even though it is only the button release that we're concerned with,
// we still handle the button press event since anything that we don't
// handle is passed along to OSG. We don't want button press events
// to be passed into OSG when the user is holding down the shift
// key or the control key, so we stop that by returning true
// when either key is held down and the right mouse button is clicked.
bool on_button_event(GdkEventButton* event)
{
    if ( event->button == 3 and ( event->state & GDK_SHIFT_MASK or event->state & GDK_CONTROL_MASK ) )
    {
        // We get here if it's either a button press or release, but we'll
        // only pop up the menu on a release
        if ( event->type == GDK_BUTTON_RELEASE )
        {
            Gtk::Menu* menu = Gtk::manage(new Gtk::Menu());
            Gtk::MenuItem * wp = new Gtk::MenuItem("Add Waypoint");
            wp->signal_activate().connect(sigc::bind<GdkEventButton>(sigc::ptr_fun(&on_addWp_clicked), *event));
            menu->append(*Gtk::manage(wp));

            Gtk::MenuItem * center_vehicle = new Gtk::MenuItem("Center on Vehicle");
            center_vehicle->signal_activate().connect(&on_center_vehicle_clicked);
            menu->append(*Gtk::manage(center_vehicle));

            Gtk::MenuItem * follow_vehicle = new Gtk::MenuItem("Follow Vehicle");
            follow_vehicle->signal_activate().connect(&on_follow_vehicle_clicked);
            menu->append(*Gtk::manage(follow_vehicle));

            Gtk::MenuItem * stop_follow_vehicle = new Gtk::MenuItem("Stop Following Vehicle");
            stop_follow_vehicle->signal_activate().connect(&on_stop_follow_vehicle_clicked);
            menu->append(*Gtk::manage(stop_follow_vehicle));
            menu->show_all();
            menu->popup(event->button, event->time);
        }

        // Whether it's press or release, if it's a right mouse event with
        // control or shift held down we return true, stopping it from
        // going on to OSG
        //
        // If we wanted to handled it _and_ let it pass into OSG we
        // could simply return false here.
        return true;
    }

    // But, since this was something other than a right mouse event with
    // a ctrl/shift key pressed, we'll just let it pass into OSG
    return false;
}

void on_open_file_clicked()
{
	Gtk::FileChooserDialog fc("Please select an OSG file to load...",
	Gtk::FILE_CHOOSER_ACTION_OPEN);

	fc.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	fc.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_ACCEPT);

	if ( fc.run() == Gtk::RESPONSE_ACCEPT )
	{
		Glib::ustring file = fc.get_filename();
		if (viewer->getSceneData()) viewer->setSceneData(NULL);
		osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(file);
		if ( model.valid() )
		{
			viewer->setSceneData(model.get());
			objectPlacer = new osgEarthUtil::ObjectPlacer(model.get());
			modelLoaded =true;
			viewer->run();
		}
	}

    //Glib::ustring file = "data/yahoo_aerial.earth";;
    //if (viewer->getSceneData()) viewer->setSceneData(NULL);
    //osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(file);
    //if ( model.valid() )
    //{
        //viewer->setSceneData(model.get());
        //modelLoaded =true;
        //viewer->run();
    //}

    viewer->getSceneData()->asGroup()->addChild(wpLines);
    viewer->getSceneData()->asGroup()->addChild(wpGroup);
}

void on_open_vehicle_clicked()
{
    //if (modelLoaded)
    //{
    //Gtk::FileChooserDialog fc("Please select an OSG Vehicle file to load...",
    //Gtk::FILE_CHOOSER_ACTION_OPEN);

    //fc.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    //fc.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_ACCEPT);

    //if ( fc.run() == Gtk::RESPONSE_ACCEPT )
    //{
    //Glib::ustring file = fc.get_filename();
    //vehicle = new MapVehicle(viewer, earthManipulator, file,"/dev/ttyUSB0", vLineGeometry, geo_label);
    //}
    //}

    if (modelLoaded)
    {
		std::string port = "/dev/ttyUSB0";
		const long int baud = 115200;
        std::string file = "data/plane.osg";
		bool fileGood, portGood;
		fileGood = portGood = false;
		if(fileExists(port.c_str())) portGood = true;
		if(fileExists(file.c_str())) fileGood = true;
		if(fileGood && portGood)
		{
        	vehicle = new MapVehicle(objectPlacer, file, port, baud, geo_label);
			viewer->getSceneData()->asGroup()->addChild(vehicle);
		}
		else
		{
			if(!portGood) std::cout<<"Vehicle comm port not valid."<<std::endl;
			if(!fileGood) std::cout<<"Vehicle model file not valid."<<std::endl;
		}
    }
}

void on_addWp_clicked(GdkEventButton & event)
{
    if (viewer->getSceneData() != NULL)
    {
        //std::cout<<"x: "<<event.x<<std::endl;
        //std::cout<<"y: "<<event.y<<std::endl;
        osgUtil::LineSegmentIntersector::Intersections intersections;
        int x, y, width, height;
        gw->getWindowRectangle(x, y, width, height);
        if (viewer->computeIntersections(event.x, height - event.y , intersections))
        {
            //std::cout<<"contains intersections"<<std::endl;
            osg::Vec3d xyz;
            earthManipulator->screenToWorld(event.x, height - event.y, viewer->getCamera()->getView(), xyz);
            osg::EllipsoidModel ellipsoid;
            double lat, lon, alt, wpDispAlt;

            earthManipulator->getSRS()->getEllipsoid()->convertXYZToLatLongHeight(xyz.x(), xyz.y(), xyz.z(), lat, lon, alt);
            if (alt<0) alt = 0; //if intercept calculate from clicking screen is calculated below 0, reset to 0;
            wpDispAlt = alt + 10;
            earthManipulator->getSRS()->getEllipsoid()->convertLatLongHeightToXYZ( lat, lon, wpDispAlt, xyz.x(), xyz.y(), xyz.z());

            //Add wp marker
            osg::ref_ptr<osg::Geode> geode = new osg::Geode;
            osg::ShapeDrawable * drawableSphere = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(xyz.x(), xyz.y(), xyz.z()), 5));
            osg::ShapeDrawable * drawableSphereOrigin = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3d(0.0, 0.0, 0.0), 10000));
            geode->addDrawable(drawableSphere);
            geode->addDrawable(drawableSphereOrigin);
            wpGroup->addChild(geode);


            //Add wpLines
            wpXYZ->push_back(xyz);
            //wpGeo->push_back(osg::Vec3d(lat, lon, alt));
            wpColors->push_back(osg::Vec4(1,0,0,1));
            wpDrawLines->setCount(wpXYZ->size());
            vehicle->addWp(lat, lon, alt);
        }
    }
}



void on_center_vehicle_clicked()
{
    if (vehicle)
    {
        osgEarthUtil::Viewpoint viewpoint;
        viewpoint = earthManipulator->getViewpoint();
        viewpoint.setFocalPoint(vehicle->getFocalPoint());
        if ( viewpoint.getRange()>500) viewpoint.setRange(500);
        earthManipulator->setViewpoint(viewpoint, 0);
    }
}

void on_follow_vehicle_clicked()
{
    if (vehicle) 
	{
		osgEarthUtil::Viewpoint viewpoint;
		viewpoint = earthManipulator->getViewpoint();
		viewpoint.setFocalPoint(vehicle->getFocalPoint());
		if ( viewpoint.getRange()>500) viewpoint.setRange(500);
			
		earthManipulator->setTetherNode(vehicle->getMatrixTransformNode());
		earthManipulator->setViewpoint(viewpoint, 0);
	}
}

void on_stop_follow_vehicle_clicked()
{
	earthManipulator->setTetherNode(NULL);
}

void on_start_trace_clicked()
{
    if ( start_stop_button->get_active())
    {
        if (vehicle) vehicle->trace = true;
        start_stop_button->set_label("Stop Trace");
    }
    else
    {
        if (vehicle) vehicle->trace = false;
        start_stop_button->set_label("Start Trace");
    }
}

void on_reset_trace_clicked()
{
	vehicle->clearTrace();
}

void on_reset_wp_clicked()
{
    wpXYZ->clear();
    wpGeo->clear();
    wpColors->clear();
    wpDrawLines->setCount(wpXYZ->size());
    while (wpGroup->removeChild(wpGroup->getChild(0)));
    vehicle->clearWpList();
}

void on_send_wp_clicked()
{
    vehicle->sendWpList();
}
