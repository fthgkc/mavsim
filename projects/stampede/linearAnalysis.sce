mode(-1)

// load constants file
exec constants.sce

// load scicoslab diagram to linearize the dynamics
load stampede.cos

// extract blocks
disp('extracting blocks for linearization');
dynamics=scs_m.objs(917).model.rpar;
motorLag=scs_m.objs(1061).model.rpar;

// lineriaztion of dynamics
disp('linearizing dynamics');
// vary u to find zero initial conitions
Yd = zeros(15,1)
Yd(y.V) = 2*3; // TODO why is they x 3? 1 m/s velocity
Yd(y.sog) = Yd(y.V);
[Xd,Ud,Yd,XPd] = steadycos2(dynamics,[],[],Yd,[],[1:$],[y.lat,y.lon]);
Xd=clean(Xd,1e-5);
Ud=clean(Ud,1e-5);
ugvTf = clean(ss2tf(lincos(dynamics,Xd,Ud)),1e-5);

// motor lag
motorLagTf = diag([tau_servo/(%s+tau_servo),tau_motor/(%s+tau_motor),0,0]);

sys.oltf = clean(ugvTf,1e-4)*motorLagTf;
sys.olss = minssAutoTol(tf2ss(sys.oltf),16);

// initialization
disp('beginning loop closures');
s = sys.olss;
i= 1;

// disable white color plot, because you can't see it with a white background
f = gdf();
f.color_map(8,:) = [0,0,0]; // set white to black in color map so it can be seen

// close loops
sYawOpen = minss(H.yaw_STR*s(y.yaw,u.STR));
[f,s,u,i] = closeLoopWithPlots('yaw',i,y.yaw,u.STR,s,y,u,H.yaw_STR);
sYawClosed = minss(s(y.yaw,u.yaw));

sVOpen = minss(H.V_THR*s(y.V,u.THR));
[f,s,u,i] = closeLoopWithPlots('V',i,y.V,u.THR,s,y,u,H.V_THR);
sVClosed = minss(s(y.V,u.V));

// zoh time effect on 
[f,i] = zohAnalysisPlot('yaw',i, sYawOpen, sYawClosed, [16]);
[f,i] = zohAnalysisPlot('V',i, sVOpen, sVClosed, [16]);

// plot step responses
[f,i] = stepAnalysis(180/%pi*s,'yaw',i,[10 50 180],'yaw, degrees',y,u,r);
[f,i] = stepAnalysis(s,'V',i,[.1 1 6],'V, m/s',y,u,r);
