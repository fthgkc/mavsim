function [f,i] = zohAnalysisPlot(name,i,sOpen,sClosed,rates)
	f=scf(i); clf(i);
	f.figure_name=name + ' zero order hold effect, open/ closed loop';
	f.figure_size = [1200,600];
	set_posfig_dim(f.figure_size(1),f.figure_size(2));
	ratesLegend = [];
	if (size(rates,2) < 8)
		for i=1:size(rates,2)
			ratesLegend(i,1) = [string(rates(1,i)) + ' Hz'];
		end
	else
		ratesLegend = '';
	end

	// plots
	subplot(1,2,1);
	bode(zohPade(rates)*sOpen,0.01,99,.01,ratesLegend)
	subplot(1,2,2);
	bode(zohPade(rates)*sClosed,0.01,99,.01)
	xs2eps(i,name+'_zoh_analysis');
	i = i +1;
endfunction

