function [x,y,typ]=insErrorDynamics(job,arg1,arg2)
//
// gpsIns.sci
//
// USAGE:
//
// output 1: F
// output 2: G
//
// input 1: (input u1)
//  [1] fn (unit distance/s^2) (inertial)
//  [2] fe (unit distance/s^2) (inertial)
//  [3] fd (unit distance/s^2) (inertial)
//	default unit distance is meters
// input 2: (input u2)
//  [1]  Lat 	(rad)
//  [2]  Phi 	(rad)
//  [3]  Theta 	(rad)
//  [4]  Psi 	(rad)
//  [5]  Vn 	(unit distance/s)
//  [6]  Ve 	(unit distance/s)
//  [7]  Vd 	(unit distance/s)
//	default unit distance is meters
//
// AUTHOR:
//
// Copyright (C) Alan Kim 2011
//
// insErrorDynamics.sci is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// insErrorDynamics.sci is distributed in the hope that it will be useful, but
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
				'R (unit distance)'];
			[ok,Omega,R]=..
				getvalue('Set Planet Parameters',labels,..
				list('vec',1,'vec',1),exprs);
			if ~ok then break,end
				graphics.exprs=exprs;
			[model,graphics,ok]=check_io(model,graphics,[3;7],[9;6],[1],[])
			if ok then
				model.rpar=[Omega,R];
				graphics.exprs=exprs;
				x.graphics=graphics;
				x.model=model;
				break
			end
		end
	case 'define' then
		// set model properties
		model=scicos_model()
		model.sim=list('sci_insErrorDynamics',4)
		model.evtin=[1];
		model.in=[3;7];
		model.out=[9;6];
		model.out2=[9;6];
		model.blocktype='c';
		model.dep_ut=[%t %f];

		// gpsIns parameters
		Omega = 7.292115e-5
		R=6378137;
		
		model.rpar=[Omega,R];
		
		// initialize strings for gui
		exprs=[
			strcat(sci2exp(Omega)),..
			strcat(sci2exp(R))];
;

		// setup icon
	  	gr_i=['xstringb(orig(1),orig(2),''insErrorDynamics'',sz(1),sz(2),''fill'');']
	  	x=standard_define([5 2],model,exprs,gr_i)
	end
endfunction

// vim:ts=4:sw=4
