n=x_choose(['JSBSim Backside Autopilot Design';'JSBSim Trim';'Visualization Test'],'oooark block demos')

if (n==1)
	scicos(oooarkPath+'demos/JSBSimBackside.cos')
elseif (n==2)
	scicos(oooarkPath+'demos/JSBSimTrim.cos')
elseif (n==3)
	scicos(oooarkPath+'demos/VisTest.cos')
else
	disp('unknown block')
end
