function [x,y,typ]=insErrorDynamics(job,arg1,arg2)
//
// insErrorDynamics.sci
//
// USAGE:
//
// input 1: (imu data)
//  [1] wx (rad/s) 		(body)
//  [2] wy (rad/s)		(body)
//  [3] wz (rad/s)	    (body)
//  [4] fx (unit distance/s^2) (body)
//  [5] fy (unit distance/s^2) (body)
//  [6] fz (unit distance/s^2) (body)
//	default unit distance is meters
//
// input 2: (input u2)
//  [1]  Phi 	(rad)
//  [2]  Theta 	(rad)
//  [3]  Psi 	(rad)
//  [4]  Vn 	(unit distance/s)
//  [5]  Ve 	(unit distance/s)
//  [6]  Vd 	(unit distance/s)
//  [7]  Lat 	(rad)
//  [8]  Lon 	(rad)
//  [9]  alt 	(unit distance)
//
//	default unit distance is meters
//
// mode: 0 (full state)
// 	output 1: F (10x10)
// 	output 2: G (10x6)  u: (gyros, accels)
//
// mode: 1 (attitude state)
// 	output 1: F (4x4)
// 	output 2: G (4x3)  u: (gyros, accels)
//
// mode: 2 (velocity/position state)
// 	output 1: F (6x6)
// 	output 2: G (6x3)  u: (gyros, accels)
//
// AUTHOR:
//
// Copyright (C) Alan Kim, James Goppert 2011
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
				'Re (unit distance)';..
				'state mode: full(0), attitude(1), velocity position(2)'];
			[ok,Omega,Re,stateMode,exprs]=..
				getvalue('Set Planet Parameters',labels,..
				list('vec',1,'vec',1,'vec',1),exprs);
			if ~ok then break,end
				graphics.exprs=exprs;

			// set sizes based on mode
			if stateMode==0 then
				model.out=[10;10];
				model.out2=[10;6];
				[model,graphics,ok]=check_io(model,graphics,[6;10],[10;10],[],[]);
			elseif stateMode==1 then
				model.out=[4;4];
				model.out2=[4;3];
				[model,graphics,ok]=check_io(model,graphics,[6;10],[4;4],[],[]);
			elseif stateMode==2 then
				model.out=[6;6];
				model.out2=[6;3];
				[model,graphics,ok]=check_io(model,graphics,[6;10],[6;6],[],[]);
			else 
				disp('invalid mode in insErrorDynamics');
				error('invalid mode in insErrorDynamics');
			end

			if ok then
				model.rpar=[Omega,Re];
				model.ipar=stateMode;
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
		model.evtin=[];
		model.in=[6;10];

		model.out=[10;10];
		model.out2=[10;6];
		model.blocktype='c';
		model.dep_ut=[%t %f];

		// gpsIns parameters
		Omega = 7.292115e-5
		Re=6378137;

		stateMode=0;

		
		model.rpar=[Omega,Re];
		model.ipar=stateMode;
		
		// initialize strings for gui
		exprs=[..
			strcat(sci2exp(Omega)),..
			strcat(sci2exp(Re)),..
			strcat(sci2exp(stateMode))];

		// setup icon
	  	gr_i=['xstringb(orig(1),orig(2),''insErrorDynamics'',sz(1),sz(2),''fill'');']
	  	x=standard_define([5 2],model,exprs,gr_i)
	end
endfunction

// vim:ts=4:sw=4
