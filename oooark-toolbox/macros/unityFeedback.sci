function sys = unityFeedback(y,u,G,H)
	C = zeros(length(y),size(G,1));
	for (i=1:length(y)) C(i,y(i)) = 1; end;
	D = zeros(size(G,2),size(u,2));
	for (i=1:length(u)) D(u(i),i) = 1; end;
	olss = G*D*H;
	nPoles = size(olss.A,1);
    sys=minssAutoTol((eye(size(G,1),size(G,1))+G*D*H*C)\[G,G*D*H],nPoles);
endfunction
// vim:sw=4:tw=4:expandtab
