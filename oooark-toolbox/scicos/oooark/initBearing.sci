function [x,y,typ]=initBearing(job,arg1,arg2)
//
// initBearing.sci
//
// USAGE: Calcualte initial bearing from point 1 to point 2
//
// output 1: Bearing in radians
//
// input 1: (input u1)
//  [1] Lat1
//  [2] Lon1
//  
// input 2: (input u2)
//  [1] Lat2
//  [2] Lon2
//
// AUTHOR:
//
// Copyright (C) Alan Kim 2011
//
// initBearing.sci is free software: you can redistribute it and/or modify it
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
		model.sim=list('sci_initBearing',4)
		model.evtin=[];
		model.in=[2;2];
		model.out=[1];
		model.blocktype='c';
		model.dep_ut=[%t %f];

        exprs='initBearing';

		// setup icon
	  	gr_i=['xstringb(orig(1),orig(2),''initBearing'',sz(1),sz(2),''fill'');']
	  	x=standard_define([5 2],model,exprs,gr_i)
	end
endfunction

// vim:ts=4:sw=4
