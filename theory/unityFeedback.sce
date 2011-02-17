function [ss] = unit_feedback(G,H)

[Ap,Bp,Cp,Dp] = abcd(G);
[Ac,Bc,Cc,Dc] = abcd(H);
A(1,1) = Ap-Bp * den * Dc * Cp;
A(1,2) = Bp * den * Cc;
A(2,1) = -Bc * Cp;
A(2,2) = Ac;
B(1,1) = Bp * den * Dc;
B(2,1) = Bc-Bc * Dp;
C(1,1) = Cp;
C(1,2) = 0;
D(1,1) = Dp;
ss = syslin('c',A,B,C,D);

endfunction
