function [f,i] =stepAnalysis(s,channels,i,steps,yLabels,y,u,r)

	// for the position loops
	for j=1:size(channels,1)

		channel = channels(j);
		yLabel = yLabels(j);

		yCh = evstr('y.'+channel);
		uCh = evstr('u.'+channel);
		rCh = evstr('r.'+channel);

		// setup figure
		i = i +1;
		f=scf(i); clf(i);
		f.figure_name=channel + ' step responses';
		f.figure_size = [800,600];
		set_posfig_dim(f.figure_size(1),f.figure_size(2));
		xlabel('t, seconds');
		ylabel(yLabel);

		// for  several step sizes
		for step = steps

			// reference input signal
			rSignal = struct();
			rSignal.time = 0;
			rSignal.values = zeros(1,4);
			rSignal.values(1,rCh) = step;

			// non-linear simulation
			scicos_simulate(scs_m);
			t = xSignal.time;
			yNLin = xSignal.values;

			// linear simulation
			yLin = csim('step',t,step*s(yCh,uCh))';

			// plotting
			plot(t,[yLin,yNLin(:,yCh)])
		end

		// plotting
		legend(['linear','non-linear'])
		xs2eps(i,channel+'_steps');
	end

endfunction
