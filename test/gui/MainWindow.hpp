/*
 * MainWindow.hpp
 * Copyright (C) James Goppert 2010 <james.goppert@gmail.com>
 *
 * MainWindow.hpp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MainWindow.hpp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef oooark_MainWindow_HPP
#define oooark_MainWindow_HPP

#include "ui_MainWindow.h"

namespace oooark
{

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
private slots:
	void on_actionLoad_Model_activated();
	void on_actionLoad_Map_activated();
};

} // oooark

#endif

// vim:ts=4:sw=4
