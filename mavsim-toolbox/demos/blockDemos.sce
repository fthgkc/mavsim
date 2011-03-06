n=x_choose([
'EasyStar Backside Autopilot Design';
'EasyStar Trim';
'EasyStar Hardware in the Loop (Sensor Level)';
'EasyStar Hardware in the Loop (State Level)';
'Visualization Test';
],'mavsim demos')

if (n==1)
	scicos(mavsimPath+'demos/block/JSBSimBackside.cos')
elseif (n==2)
	scicos(mavsimPath+'demos/block/JSBSimTrim.cos')
elseif (n==3)
	scicos(mavsimPath+'demos/block/JSBSimMavLinkHilState.cos')
elseif (n==4)
	scicos(mavsimPath+'demos/block/JSBSimMavLinkHilSensor.cos')
elseif (n==5)
	scicos(mavsimPath+'demos/block/VisTest.cos')
else
	disp('unknown demo')
end
