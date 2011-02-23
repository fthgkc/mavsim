function sys = unityFeedback(y,u,G,H)
	C = zeros(length(y),size(G,1));
	for (i=1:length(y)) C(i,y(i)) = 1; end;
	D = zeros(size(G,2),size(u,2));
	for (i=1:length(u)) D(u(i),i) = 1; end;
	sys=minss((eye(size(G,1),size(G,1))+G*D*H*C)\[G,G*D*H]);
endfunction
