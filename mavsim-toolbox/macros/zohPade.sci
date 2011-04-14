function sys = zohPade(controlPeriod)
// zero order hold pade approximation
// controlPeriod must be a row vector
	sys = [];
	for i=1:size(controlPeriod,2)
		sys(i,1) = (1-%s*controlPeriod(1,i)/6)/(1 + %s*controlPeriod(1,i)/3);
	end
endfunction
