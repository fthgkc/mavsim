/*
 * MapWidget.hpp
 * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 *
 * MapWidget.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MapWidget.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef oooark_MapWidget_HPP
#define oooark_MapWidget_HPP

#include "QOSGAdapterWidget.hpp"

namespace oooark
{

class MapWidget : public ViewerQT
{
public:
	MapWidget(QWidget * parent = 0, const char * name = 0,
		const QGLWidget * shareWidget = 0, WindowFlags f = 0);
    virtual ~MapWidget();

protected:

	// mouse
	virtual void mousePressEvent( QMouseEvent* event);
	virtual void mouseDoubleClickEvent( QMouseEvent* event);
	QPoint mouseClickPosition;

private:

	// view
	void showViewMenu();
	void followVehicle();
	void centerOnVehicle();
	void centerOnGroundStation();
	void stopFollowingVehicle();

	// wayopints
	virtual void showWaypointMenu();
	void addWaypoint();
	void deleteWaypoint();
	void clearWaypoints();
};

} // oooark

#endif

// vim:ts=4:sw=4
