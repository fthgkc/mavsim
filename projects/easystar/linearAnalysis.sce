mode(-1)

// load constants file
exec constants.sce;

// load scicoslab diagram to linearize the dynamics
load easystar.cos

// extract blocks
disp('extracting blocks for linearization');
//dynamics=scs_m.objs(1).model.rpar;
actuators=scs_m.objs(46).model.rpar;

// lineriaztion of dynamics
disp('linearizing dynamics');
// vary u to find zero initial conitions
//[Xd,Ud,Yd,XPd] = steadycos2(dynamics,[],[],[],[],[1:$],[],[]);
//Xd=clean(Xd,1e-5);
//Ud=clean(Ud,1e-5);
//easyStarTf = clean(ss2tf(lincos(dynamics,Xd,Ud)),1e-5);
easyStarTf = sys;
Xd = x0;
Ud = u0;

// motor lag block
disp('linearizing actuators block');
actuatorsTf = clean(ss2tf(lincos(actuators,zeros(4,1),Ud)),1e-5);

// find complete dynamics transfer function
disp('finding dynamics transfer function');
clear sys;
sys.olss = easyStarTf*actuatorsTf;

// attitude loops
disp('beginning loop closures');
s = sys.olss;
s0 = ss2tf(s);
