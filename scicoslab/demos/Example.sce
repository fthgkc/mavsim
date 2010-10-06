z = 2;
Gp = 1/((%s+3)*(%s+6));
Gc = (%s + z)/%s;
Gff = Gp*Gc;
k = 10;
Gcl = k*Gff/(1+k*Gff);
ce = denom(Gcl);
r = roots(ce);


K = 20;
ce_new = denom(Gff)+numer(Gff)*K;
roots_new = roots(ce_new);
wd = imag(roots_new);
size_wd = size(wd);
counter = 1;

while counter <= size_wd(:,1)
  if wd(counter) ~= 0
    sigma = real(roots_new(counter));
    zeta = sqrt(1-(wd(counter)^2)/(sigma^2+wd(counter)^2))
  else
  counter=counter+1;
  end
  counter=counter+1;
end

 
