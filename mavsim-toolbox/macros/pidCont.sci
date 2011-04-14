function sys = pidCont(kP,kI,kD,controlPeriod)
// continuous pid controller model, with zero order hold approximation
// controlPeriod is in seconds
	sys = zohPade(1/controlPeriod)*syslin('c',kP+kI/%s+%s*kD);
endfunction

