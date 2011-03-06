n=x_choose([
'EasyStar Backside Autopilot Design';
'EasyStar Trim';
'EasyStar Hardware in the Loop (Sensor Level)';
'EasyStar Hardware in the Loop (State Level)';
'Visualization Test';
],'oooark demos')

if (n==1)
	scicos(oooarkPath+'demos/block/JSBSimBackside.cos')
elseif (n==2)
	scicos(oooarkPath+'demos/block/JSBSimTrim.cos')
elseif (n==3)
	scicos(oooarkPath+'demos/block/JSBSimMavLinkHilState.cos')
elseif (n==4)
	scicos(oooarkPath+'demos/block/JSBSimMavLinkHilSensor.cos')
elseif (n==5)
	scicos(oooarkPath+'demos/block/VisTest.cos')
else
	disp('unknown demo')
end
