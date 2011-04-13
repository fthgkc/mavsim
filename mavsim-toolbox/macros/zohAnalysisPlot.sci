function [f,i] = zohAnalysisPlot(name,i,sOpen,sClosed,rates)
	f=scf(i); clf(i);
	f.figure_name=name + ' zero order hold effect';
	f.figure_size = [1200,600];
	set_posfig_dim(f.figure_size(1),f.figure_size(2));
	ratesLegend = [];
	for i=1:length(rates)
		ratesLegend(i,1) = string(rates(i)) + ' Hz';
	end

	// plots
	subplot(1,2,1);
	bode(sClosed*zohPade(rates),0.01,99,.01,ratesLegend)
	subplot(1,2,2);
	bode([sOpen*zohPade(rates),0.01,99,.01,ratesLegend)
	xs2eps(i,name+'_zoh_analysis');
	i = i +1;
endfunction

