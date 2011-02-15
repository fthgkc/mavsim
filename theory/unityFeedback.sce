function ss = unityFeedback(G,H)
  [Ap,Bp,Cp,Dp] = abcd(G);
  [Ac,Bc,Cc,Dc] = abcd(H);
  den=inv(Dp*Dc+eye(size(Dp,1),size(Dc,2)));
  //A=[(Ap)-den*(Bp*Dc*Cp),(Bp*Cc-den*(Bp*Dc*Dp*Cc));-den*(Bc*Cp),(Ac-den*Bc*Dp*Cc)];
  //B=[(Bp*Dc-den*Bp*Dc*Dp*Dc);Bc-den*Bc*Dp*Dc];
  //C=[(Cp-den*Dp*Dc*Cp),(Dp*Cc-den*Dp*Dc*Dp*Cc)];
  //D=(Dp*Dc-den*Dp*Dc*Dp*Dc);
  A=[(Ap)-(Bp*Dc*Cp),(Bp*Cc-(Bp*Dc*Dp*Cc));-(Bc*Cp),(Ac-Bc*Dp*Cc)];
  B=[(Bp*Dc-Bp*Dc*Dp*Dc);Bc-Bc*Dp*Dc];
  C=[(Cp-Dp*Dc*Cp),(Dp*Cc-Dp*Dc*Dp*Cc)];
  D=(Dp*Dc-Dp*Dc*Dp*Dc);
  ss = syslin('c',A,B,C,D);
endfunction
