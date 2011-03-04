function [x,y,typ]=eom6Dof(job,arg1,arg2)
//
// eom6Dof.sci
//
// USAGE:
//
// Input: 
//  u1: F_b_T(0,0) , F_b_T(1,0) , F_b_T(2,0) 
//  u2: M_b_T(0,0) , M_b_T(1,0) , M_b_T(2,0)
//  u3: F_w_A(0,0) , F_w_A(1,0) , F_w_A(2,0)
//  u4: M_b_A(0,0) , M_b_A(1,0) , M_b_A(2,0)
//  u5: Jx, Jy, Jz, Jxy, Jxz, Jyz, g, m 
//  u6: (state) Vt, alpha, theta, wy, h, beta, phi, wx, psi, wz 
// Output:
//  y1: (state derivative)
//
// AUTHOR:
//
// Copyright (C) James Goppert Nicholas Metaxas 2011
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
			labels=['frame: Wind(0), Body(1)'];
			[ok,frame,exprs]=..
				getvalue('Set Frame',labels,..
				list('vec',1),exprs);
			if ~ok then break,end
				graphics.exprs=exprs;
			
           
			model.out=[nOut];
			model.in=[nIn];
			[model,graphics,ok]=check_io(model,graphics,nIn,nOut,[],[])
			if ok then
				model.ipar=frame;
				graphics.exprs=exprs;
				x.graphics=graphics;
				x.model=model;
				break
			end
		end
	case 'define' then
		// set model properties
		model=scicos_model();
    
	    model.sim=list('sci_eom6Dof',4);
        
       	nOut=10;
	   	nIn=[3;3;3;3;8;10]
                
		model.in=nIn;
		model.out=nOut;
		model.blocktype='c';
		model.dep_ut=[%t %f];

		// gpsIns parameters
		Omega = 7.292115e-5;
		Re=6378137;
		frame=0; // full state
		
		model.ipar=frame;
		
		// initialize strings for gui
		exprs=[..
			strcat(sci2exp(frame))];

		// setup icon
	  	gr_i=['xstringb(orig(1),orig(2),[''EOM 6DOF''],sz(1),sz(2),''fill'');']
	  	x=standard_define([5 2],model,exprs,gr_i)
	end
endfunction

// vim:ts=4:sw=4
