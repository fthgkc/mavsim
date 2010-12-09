function [x,y,typ]=gpsIns(job,arg1,arg2)
//
// gpsIns.sci
//
// USAGE:
//
// output 1: (state x) 
//  [1]  Lat
//  [2]  Lon
//  [3]  Alt
//  [4]  roll
//  [5]  pitch
//  [6]  yaw
//  [7]  Vn
//  [8]  Ve
//  [9]  Vd
//
// input 1: (input u1)
//  [1] fbi (inertial)
//  [2] wbi (inertial)
//
// input 2: (input u2)
//  [1]  Lat
//  [2]  Lon
//  [3]  Alt
//  [4]  Phi
//  [5]  Theta
//  [6]  Psi
//  [7]  Vn
//  [8]  Ve
//  [9]  Vd
//
// AUTHOR:
//
// Copyright (C) Brandon Wampler 2010 <bwampler@users.sourceforge.net>
//
// gpsIns.sci is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// gpsIns.sci is distributed in the hope that it will be useful, but
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
				'Position sigma (m)';..
				'Alt sigma (m)';..
				'Velocity sigma (m)';..
				'Accel. sigma (m/s^2)';..
				'Gyro sigma (rad/s)'];
			[ok,SigmaPos,SigmaAlt,SigmaVel,SigmaAccel,SigmaGyro,exprs]=..
				getvalue('Set GpsIns Parameters',labels,..
				list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1),exprs);
			if ~ok then break,end
			n=size(x0,1)
			model.out=[n;n];
			[model,graphics,ok]=check_io(model,graphics,[4],[n;n],[],[])
			if ok then
				model.state=[x0];
				model.rpar=[];
				graphics.exprs=exprs;
				x.graphics=graphics;
				x.model=model;
				break
			end
		end
	case 'define' then
		// set model properties
		model=scicos_model()
		model.sim=list('sci_gpsIns',4)
		model.in=[6;9]
		model.out=[9]
		model.blocktype='c'
		model.dep_ut=[%f %t]

		// gpsIns parameters
		SigmaPos=10;
		SigmaAlt=5;
		SigmaVel=1;
		SigmaAccel=.001;
		SigmaGyro=.002;
		model.rpar=[SigmaPos,SigmaAlt,SigmaVel,SigmaAccel,SigmaGyro];
		
		// intial state
		x0=[]

		// save state
		model.state=x0;

		// initialize strings for gui
		exprs=[
			strcat(sci2exp(SigmaPos)),..
			strcat(sci2exp(SigmaAlt)),..
			strcat(sci2exp(SigmaVel)),..
			strcat(sci2exp(SigmaAccel)),..
			strcat(sci2exp(SigmaGyro))];

		// setup icon
	  	gr_i=['xstringb(orig(1),orig(2),''GpsIns'',sz(1),sz(2),''fill'');']
	  	x=standard_define([5 2],model,exprs,gr_i)
	end
endfunction

// vim:ts=4:sw=4
