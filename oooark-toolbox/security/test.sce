clear clc
load SCI/macros/scicos/lib
exec (loadpallibs,-1)
cd /hsl/homes/awkim/Projects/oooark/scicoslab/security

load Untitled.cos
state = %cpr.state;
tf = 50;
tol = [0.000001,0.000001,1.000D-10,31,0,0];
scs_m.props.tf=tf;

values = linspace(0,10,3);

for i = 1:3
  x = values(i);
  Info = scicos_simulate(scs_m);
  savematfile('Data\time'+string(i),'A.time');
  savematfile('Data\values'+string(i),'A.values');
  clear A
end
  
  
  
  
  
