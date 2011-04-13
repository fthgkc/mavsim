function sys = zohPade(controlPeriod)
// zero order hold pade approximation
	sys = (1-%s*controlPeriod/6)/(1 + %s*controlPeriod/3);
endfunction
