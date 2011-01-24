/*
 * definitions.cpp
 * Copyright (C) James Goppert 2010 <jgoppert@users.sourceforge.net>
 *
 * This file is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

extern "C" {

namespace scicos
{

int evtPortNumToFlag(int portNum)
{
	if (portNum > 0) return 1 << (portNum-1); //  return the bit flag equivalent
	else return -9; // if port is not there return a value that won't match
}

}

} // extern C

// vim:ts=4:sw=4:expandtab
