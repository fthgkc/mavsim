function [x,y,typ]=ZOHold(job,arg1,arg2)
// Copyright INRIA
//x=[];y=[];typ=[];
//select job
//case 'plot' then
  //standard_draw(arg1)
//case 'getinputs' then
  //[x,y,typ]=standard_inputs(arg1)
//case 'getoutputs' then
  //[x,y,typ]=standard_outputs(arg1)
//case 'getorigin' then
  //[x,y]=standard_origin(arg1)
//case 'set' then
  //x=arg1;
  //graphics=arg1.graphics;exprs=graphics.exprs
  //model=arg1.model;
  //while %t do
    //[ok,a,inh,reset,exprs]=getvalue('Set 1/z block parameters',..
	//['Initial condition';'Inherit (no:0, yes:1)';'Reset (no:0, yes:1)'],...
				  //list('mat',[-1 -2],'vec',1,'vec',1),exprs)
    //if ~ok then break,end
    //if isempty(a) then a=0,end
    //if size(a,"*")==1 then 
      //out=[-1,-2],
    //else
      //out=[size(a,1) size(a,2)];
    //end
	//if reset then	
		//in=[out;out];
	//else 
		//in = out
	//end
    //if do_get_type(a)==1 then
      //ot=-1
    //else
      //ot=do_get_type(a)
    //end
    //it=ot
    //if ok then
      //[model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),ones(1-inh,1),[])
    //end
    //if ok then
      //graphics.exprs=exprs;
      //x.graphics=graphics;x.model=model
      //break
    //end
  //end
//case 'compile'
  //model=arg1
  //sz=[model.in model.in2]
  //typ=model.intyp
  //z=model.dstate
  //if size(z,'*')==1 then 
      //z(1:sz(1),1:sz(2))=z
////  elseif ~isequal(sz,size(z)) then
////      error("state has size "+sci2exp(size(z))+" but input/output has size "+sci2exp(sz))
////  elseif do_get_type(z)>1 then
////     if ~isequal(do_get_type(z),typ) then
////      error("state has type "+string(do_get_type(z))+" but input/output has type "+sci2exp(typ))
////     end
  //elseif do_get_type(z)==1 then
     //select typ
     //case 2
       //z=z+0*%i
     //case 3
       //z=int32(z)
     //case 4
       //z=int16(z)
     //case 5
       //z=int8(z)
     //case 6
       //z=uint32(z)
     //case 7
       //z=uint16(z)
     //case 8
       //z=uint8(z)
     //case 9
       //z=z>0
     //end
  //end
  //if size(z,'*')==1 & typ==1 then
    //model.sim=list('dollar4',4);
    //model.dstate=z(:);
    //model.odstate=list();
  //else
    //model.sim=list('dollar4_m',4)
    //model.odstate=list(z);
    //model.dstate=[];
  //end
  //x=model

//case 'define' then
  //z=0
  //a=0
  //inh=0
  //reset=0
  //exprs=string([z;inh])
  //model=scicos_model()
  //model.sim=list('dollar4',4) 
  //model.in=-1
  //model.in2=-2
  //model.out=-1
  //model.out2=-2
  //model.evtin=[1-inh;reset]
  //model.dstate=z
  //model.blocktype='d'
  //model.dep_ut=[%f %f]
  //model.rpar=a
  //gr_i='xstringb(orig(1),orig(2),''1/z'',sz(1),sz(2),''fill'')'
  //x=standard_define([2 2],model,exprs,gr_i)
//end
endfunction

