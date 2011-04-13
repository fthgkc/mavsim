function sys = pidCont(kP,kI,kD,controlPeriod)
// continuous pid controller model, with zero order hold approximation
	sys = zohPade(controlPeriod)*syslin('c',kP+kI/%s+%s*kD);
endfunction

