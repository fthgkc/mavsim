function [f,s,u,fIndex] = closeLoopWithPlots(name,fIndex,yi,ui,s,y,u,H)
	s0 = ss2cleanTf(s);
	[s,u] = closeLoop(yi,ui,s,y,u,H);
	s1 = ss2cleanTf(s);

	f=scf(fIndex); clf(fIndex);
	f.figure_name=name;
	f.figure_size = [1200,600];
	set_posfig_dim(f.figure_size(1),f.figure_size(2));

	subplot(1,3,1)
	bode([s0(yi,ui);H*s0(yi,ui);s1(yi,ui)],..
	0.01,99,.01,['open loop';'compensated open loop';'compensated closed loop'])

	subplot(1,3,2)
	evans(s0(yi,ui),100);
	title(gca(),'Uncompensated Root Locus');
	mtlb_axis([-10,10,-10,10]);

	subplot(1,3,3)
	evans(H*s0(yi,ui),10);
	title(gca(),'Compensated Root Locus');
	mtlb_axis([-10,10,-10,10]);

	// save
	xs2eps(fIndex,name);
	fIndex = fIndex +1;
endfunction
