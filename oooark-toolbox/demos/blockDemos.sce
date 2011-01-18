n=x_choose([
'EasyStar Backside Autopilot Design';
'EasyStar Trim';
'EasyStar Hardware in the Loop (Sensor Level)';
'EasyStar Hardware in the Loop (State Level)';
'Visualization Test';
],'oooark block demos')

if (n==1)
	scicos(oooarkPath+'demos/JSBSimBackside.cos')
elseif (n==2)
	scicos(oooarkPath+'demos/JSBSimTrim.cos')
elseif (n==3)
	scicos(oooarkPath+'demos/JSBSimMavLinkHilState.cos')
elseif (n==4)
	scicos(oooarkPath+'demos/JSBSimMavLinkHilSensor.cos')
elseif (n==5)
	scicos(oooarkPath+'demos/VisTest.cos')
else
	disp('unknown block')
end
