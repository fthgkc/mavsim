function ss = unityFeedback(G,H)
  	[Ap,Bp,Cp,Dp] = abcd(G);
  	[Ac,Bc,Cc,Dc] = abcd(H);
  	den=1/(eye(size(Dc,1),size(Dp,2))-Dc * Dp);
	A = [Ap-Bp * den * Dc * Cp,Bp * den * Cc;
		Bc * Dp * den * Dc * Cp-Bc * Cp,Ac-Bc * Dp * den * Cc];
	B = [Bp * den * Dc;Bc-Bc * Dp * den * Dc];
	C = [Cp-Dp * den * Dc * Cp,Dp * den * Cc];
	D = [Dp * den * Dc];
  	ss = syslin('c',A,B,C,D);
endfunction
