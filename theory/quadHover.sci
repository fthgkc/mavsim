clc; clear;
mode(-1);

function qwd = quad_hover_design

// load airframe
exec mikrokopter.sci

// solve for pitch angle at trim;
theta=0;

// solve for T_sumSq at trim
T_sumSq_trim= .4; // use equation in maxima
qwd.trim.T_sumSq = T_sumSq_trim;
T_sumSq=T_sumSq_trim;
dutycycle_Lc=T_sumSq/4;
dutycycle_L=T_sumSq/4;
dutycycle_Rc=T_sumSq/4;
dutycycle_R=T_sumSq/4;
dutycycle_Bc=T_sumSq/4;
dutycycle_B=T_sumSq/4;
dutycycle_Fc=T_sumSq/4;
dutycycle_F=T_sumSq/4;

// include dynamics
exec quad_hover_dynamics.sci;

qwd.open.ss = syslin('c',F_body_quad,G_body_quad,C_body_quad,D_body_quad);
qwd.open.tf = clean(ss2tf(qwd.open.ss),1e-8);

// state
qwd.names.x(1) = "U";
qwd.names.x(2) = "W";
qwd.names.x(4) = "wy";
qwd.names.x(5) = "h";
qwd.names.x(6) = "V";
qwd.names.x(7) = "phi";
qwd.names.x(8) = "wx";
qwd.names.x(9) = "psi";
qwd.names.x(10) = "wz";
qwd.names.x(11) = "dcL";
qwd.names.x(12) = "dcR";
qwd.names.x(13) = "dcF";
qwd.names.x(14) = "dcB";

// output
qwd.names.y(1) = "U";	 	//forward velocity
qwd.names.y(2) = "W";    	//vertical velocity
qwd.names.y(3) = "theta";	//pitch (angle)
qwd.names.y(4) = "wy"; 		//pitch rate
qwd.names.y(5) = "h";		//altitude
qwd.names.y(6) = "V";		//left-right
qwd.names.y(7) = "phi";		//roll (angle)
qwd.names.y(8) = "wx";		//roll rate
qwd.names.y(9) = "psi";		//heading (angle)
qwd.names.y(10) = "wz";		//yaw rate

// input
qwd.names.u(1) = "sum_sq";
qwd.names.u(2) = "F_B_sq";
qwd.names.u(3) = "L_R_sq";
qwd.names.u(4) = "RL_FB_sq";

// include unityFeedback function
exec unityFeedback.sci;

disp("Closing Loop 1");
qwd.Loop(1).H = diag([
		1 + 0/%s + 0*%s/(%s+20)); 	// vertical velocity error to power 
		1 + 0/%s + 0*%s/(%s+20)); 	// forward velocity error to power
		1 + 0/%s + 0*%s/(%s+20)); 	// side velocity error to power
		1 + 0/%s + 0*%s/(%s+20)); 	// yaw rate error to power
]); 
qwd.Loop(1).u = [1,2,3,4];
qwd.Loop(1).y = [2,1,6,10];
qwd.Loop(1).olss=qwd.open.ss;
qwd.Loop(1).oltf=clean(ss2tf(qwd.Loop(1).olss));
qwd.Loop(1).clss = unityFeedback(qwd.Loop(1).olss,qwd.Loop(1).H,qwd.Loop(1).y,qwd.Loop(1).u);
qwd.Loop(1).cltf = clean(ss2tf(qwd.Loop(1).clss)); 
qwd.names.u(5) = "W - vertical velocity command";
qwd.names.u(6) = "U - fwd velocity command";
qwd.names.u(7) = "V - side velocity command";
qwd.names.u(8) = "WZ - yaw rate command";
scf(1); clf(1);
subplot(1,4,1); bode(qwd.Loop
(1).cltf(2,5),.01,100,.1,qwd.names.u(5));
subplot(1,4,2); bode(qwd.Loop(1).cltf(1,6),.01,100,.1,qwd.names.u(6));
subplot(1,4,3); bode(qwd.Loop(1).cltf(6,7),.01,100,.1,qwd.names.u(7));
subplot(1,4,4); bode(qwd.Loop(1).cltf(10,8),.01,100,.1,qwd.names.u(8));
disp("Loop 1 Closed");

disp("Closing Loop 2");
qwd.Loop(2).H = diag([
		0.5 + 0/%s + 0*%s/(%s+20); 	// roll error to roll rate command
		0.5 + 0.1/%s + 0*%s/(%s+20); 	// velocity error to pitch rate command
		0.5 + 0/%s + 0*%s/(%s+20)  	// yaw error to yaw rate command
]); 
qwd.Loop(2).u = [5,8];
qwd.Loop(2).y = [5,9];
qwd.Loop(2).olss=qwd.Loop(1).clss;
qwd.Loop(2).oltf=qwd.Loop(1).cltf;
qwd.Loop(2).clss = unityFeedback(qwd.Loop(1).clss,qwd.Loop(2).H,qwd.Loop(2).y,qwd.Loop(2).u);
qwd.Loop(2).cltf = clean(ss2tf(qwd.Loop(2).clss)); 
qwd.names.u(9) = "W - vertical velocity command";
qwd.names.u(10) = "psi - yaw command";
scf(2); clf(2);
subplot(1,4,1); bode(qwd.Loop(2).cltf(5,9),.01,100,.1,qwd.names.u(9));
subplot(1,4,2); bode(qwd.Loop(2).cltf(2,10),.01,100,.1,qwd.names.u(10));
subplot(1,4,3); bode(qwd.Loop(2).cltf(7,11),.01,100,.1,qwd.names.u(11));
disp("Loop 2 Closed");

// plots
//scf(1);

// wx
//sys.i = 2; sys.j = 2; sys.name = "wx rate";
//sys.open.ss=open.ss(6,3);
//sys.open.tf=clean(ss2tf(sys.open.ss),1e-8);
//sys.closed.ss = closed.ss(sys.i,sys.j);
//sys.closed.tf = clean(ss2tf(sys.closed.ss));
//sys.phase_margin = min(180 + p_margin(sys.closed.ss));
//sys.gain_margin = g_margin(sys.closed.ss);
//wx=sys;

// plots
//sys = wx; disp(sys.name); disp(sys); if (sys.closed.tf ~= 0) subplot(1,3,sys.i); bode(sys.closed.ss,.01,1000,.1,sys.name); end
//sys = wz; disp(sys.name); disp(sys); if (sys.closed.tf ~= 0) subplot(1,3,sys.i); bode(sys.closed.ss,.01,1000,.1,sys.name); end
endfunction

qwd= quad_hover_design();
