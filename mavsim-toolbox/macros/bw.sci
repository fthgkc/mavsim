function val=bw(sys,dB)
	
	if argn(2)==1
		dB=-3;
	end

	function y=mag(s)
		y=norm(horner(ss2tf(minss(sys)),%i*s*2*%pi));
	endfunction

	function [y,ind]=magError(s,ind); 
		y=abs(mag(s)-10^(-dB/20));
	endfunction

	[f,val] = optim(list(NDcost,magError),10)
	if(val<0) val=-val; end
endfunction
