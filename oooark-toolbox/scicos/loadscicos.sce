mode(-1);
lines(0);

oooarkDataPath='/hsl/homes/awkim/Projects/oooark/data'

// whether or not to configure for final installation
scicoslabInstallSettings = 0;

stdBlocks=['sci_insErrorDynamics';'sci_zeroOrderHold'];
simgearBlocks=['sci_flightGearComm'];
osgBlocks=['sci_jet';'sci_car';'sci_quad';'sci_plane'];
jsbsimBlocks=['sci_jsbsimTrim';'sci_jsbsimComm'];
lapackBlocks=['sci_gpsIns'];
mavlinkBlocks=['sci_mavlinkHilSensor';'sci_mavlinkHilState';'sci_mavlinkHilTracker'];
blocks=[stdBlocks;jsbsimBlocks;mavlinkBlocks;simgearBlocks;osgBlocks;lapackBlocks]';

if (scicoslabInstallSettings) then
	link('/hsl/homes/awkim/Projects/oooark/data/liboooarkScicos.so',blocks,'c');
else
	link('/hsl/homes/awkim/Projects/oooark/build/src/scicos/liboooarkScicos.so',blocks,'c');
end

genlib('lib_oooark',pwd()+'/oooark');
if (~scicoslabInstallSettings) then create_palette('oooark'); end
add_palette('oooark',pwd()+'/oooark/oooark.cosf');

clear blocks stdBlocks simgearBlocks osgBlocks jsbsimBlocks lapackBlocks mavlinkBlocks
