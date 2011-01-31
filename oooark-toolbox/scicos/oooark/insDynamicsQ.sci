function [x,y,typ]=insDynamicsQ(job,arg1,arg2)
//
// gpsIns.sci
//
// USAGE:
//
// input 1: (imu data)
//  [1] fx (unit distance/s^2) (body)
//  [2] fy (unit distance/s^2) (body)
//  [3] fz (unit distance/s^2) (body)
//  [4] wx (rad/s) 		(body)
//  [5] wy (rad/s)		(body)
//  [6] wz (rad/s)	    (body)
//	default unit distance is meters
//
// input 2: (gravity model)
//  [1]  g       (unit distance/s^2) (body)
//
// input 3: (state) 
//  [1]  a      quaternion
//  [2]  b 		quaternion
//  [3]  c		quaternion
//  [4]  d      quaternion
//  [5]  Vn 	(unit distance/s)
//  [6]  Ve 	(unit distance/s)
//  [7]  Vd 	(unit distance/s)
//  [8]  Lat 	(rad)
//  [9]  Lon 	(rad)
// [10]  alt 	(unit distance)
//
// output 1: (state derivative)
//  [1]  d/dt a     quaternion rate
//  [2]  d/dt b 	quaternion rate
//  [3]  d/dt c		quaternion rate
//  [4]  d/dt d     quaternion rate
//  [5]  d/dt Vn 	(unit distance/s^2)
//  [6]  d/dt Ve 	(unit distance/s^2)
//  [7]  d/dt Vd 	(unit distance/s^2)
//  [8]  d/dt Lat 	(rad/s)
//  [9]  d/dt Lon 	(rad/s)
// [10]  d/dt alt 	(unit distance/s)
//
//	default unit distance is meters
//
// AUTHOR:
//
// Copyright (C) James Goppert 2011
//
// This file is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This file is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.
//
x=[];y=[];typ=[];

select job
	case 'plot' then
	 	standard_draw(arg1)
	case 'getinputs' then
	 	[x,y,typ]=standard_inputs(arg1)
	case 'getoutputs' then
	 	[x,y,typ]=standard_outputs(arg1)
	case 'getorigin' then
	 	[x,y]=standard_origin(arg1)
	case 'set' then
		x=arg1;
		graphics=arg1.graphics;exprs=graphics.exprs
		model=arg1.model;
		while %t do
			labels=[..
				'Omega (rad/s)';..
				'Re (unit distance)'];
			[ok,Omega,Re,exprs]=..
				getvalue('Set Planet Parameters',labels,..
				list('vec',1,'vec',1),exprs);
			if ~ok then break,end
				graphics.exprs=exprs;
			[model,graphics,ok]=check_io(model,graphics,[6;1;10],[10],[],[])
			if ok then
				model.rpar=[Omega,Re];
				graphics.exprs=exprs;
				x.graphics=graphics;
				x.model=model;
				break
			end
		end
	case 'define' then
		// set model properties
		model=scicos_model()
		model.sim=list('sci_insDynamicsQ',4)
		model.in=[6;1;10];
		model.out=[10];
		model.blocktype='c';
		model.dep_ut=[%f %t];

		// gpsIns parameters
		Omega = 7.292115e-5
		Re=6378137;
		
		model.rpar=[Omega,Re];
		
		// initialize strings for gui
		exprs=[
			strcat(sci2exp(Omega)),..
			strcat(sci2exp(Re))];
;

		// setup icon
	  	gr_i=['xstringb(orig(1),orig(2),[''ins dyanmics'';''quaternions''],sz(1),sz(2),''fill'');']
	  	x=standard_define([5 2],model,exprs,gr_i)
	end
endfunction

// vim:ts=4:sw=4
