function [x,y,typ]=insQmagH(job,arg1,arg2)
//
// insQmagH.sci
//
// USAGE:
//
// output 1: H (3x10)
//
// input 1: (input u1)
//  [1] Bx  (body)
//  [2] By  (body)
//  [3] Bz  (body)
//	default unit distance is meters
// input 2: (input u2)
//  [1]  a      quaternion
//  [2]  b 	quaternion
//  [3]  c	quaternion
//  [4]  d      quaternion
//  [4]  Vn 	(unit distance/s)
//  [5]  Ve 	(unit distance/s)
//  [6]  Vd 	(unit distance/s)
//  [7]  Lat 	(rad)
//  [8]  Lon 	(rad)
//  [9]  alt 	(unit distance)

//	default unit distance is meters
//
// AUTHOR:
//
// Copyright (C) Alan Kim 2011
//
// insErrorDynamicsQ.sci is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// insErrorDynamicsQ.sci is distributed in the hope that it will be useful, but
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

	case 'define' then
		// set model properties
		model=scicos_model()
		model.sim=list('sci_insQmagH',4)
		model.evtin=[];
		model.in=[3;10];
		model.out=[3];
		model.out2=[10];
		model.blocktype='c';
		model.dep_ut=[%t %f];

        exprs='insQmagH';

		// setup icon
	  	gr_i=['xstringb(orig(1),orig(2),''insQmagH'',sz(1),sz(2),''fill'');']
	  	x=standard_define([5 2],model,exprs,gr_i)
	end
endfunction

// vim:ts=4:sw=4
