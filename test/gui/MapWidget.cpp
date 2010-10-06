/*
 * MapWidget.cpp
 * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 *
 * MapWidget.cpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MapWidget.cpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MapWidget.hpp"
#include <QMenu>
namespace oooark
{

MapWidget::MapWidget(QWidget * parent, const char * name, 
	const QGLWidget * shareWidget, WindowFlags f):
            ViewerQT( parent, name, shareWidget, f )
{
}

MapWidget::~MapWidget()
{
}

void MapWidget::mousePressEvent( QMouseEvent* event )
{
 	int button = 0;
	mouseClickPosition = event->globalPos();
    switch(event->button())
    {
        case(Qt::LeftButton): 
			button = 1;
			if (event->modifiers() == Qt::ShiftModifier)
			{
				showWaypointMenu();
			}
			break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton):
			button = 3;
			if (event->modifiers() == Qt::ShiftModifier)
			{
				showViewMenu();
			}
			break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}

void MapWidget::mouseDoubleClickEvent( QMouseEvent* event )
{
 	int button = 0;
	mouseClickPosition = event->globalPos();
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);
}

// view

void MapWidget::showViewMenu()
{
    QMenu menu;
    menu.addAction("Follow Vehicle",this,SLOT(followVehicle()));
    menu.addAction("Center on Vehicle",this,SLOT(centerOnVehicle()));
    menu.addAction("Center on Ground Station",this,SLOT(centerOnGroundStation()));
    menu.addAction("Stop Following Vehicle",this,SLOT(stopFollowingVehicle()));
	menu.exec(mouseClickPosition);
}

void MapWidget::followVehicle()
{
	std::cout << "follow vehicle" << std::endl;
}

void MapWidget::centerOnVehicle()
{
	std::cout << "center on vehicle" << std::endl;
}

void MapWidget::centerOnGroundStation()
{
	std::cout << "center on groundstation" << std::endl;
}

void MapWidget::stopFollowingVehicle()
{
	std::cout << "stop following vehicle" << std::endl;
}

// waypoints

void MapWidget::showWaypointMenu()
{
    QMenu menu;
    menu.addAction("Add Waypoint",this,SLOT(addWaypoint()));
    menu.addAction("Delete Waypoint",this,SLOT(deleteWaypoint()));
    menu.addAction("Clear Waypoints",this,SLOT(clearWaypoints()));
	menu.exec(mouseClickPosition);
}

void MapWidget::addWaypoint()
{
	std::cout << "add waypoint" << std::endl;
}

void MapWidget::deleteWaypoint()
{
	std::cout << "delete wayopint" << std::endl;
}

void MapWidget::clearWaypoints()
{
	std::cout << "clear waypoint" << std::endl;
}

} // oooark


// vim:ts=4:sw=4
