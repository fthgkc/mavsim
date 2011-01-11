mode(-1);

simgearBlocks=['sci_flightGearComm'];
osgBlocks=['sci_jet','sci_car','sci_quad','sci_plane']';
jsbsimBlocks=['sci_jsbsimTrim','sci_jsbsimComm']';
lapackBlocks=['sci_gpsIns']';
mavlinkBlocks=['sci_mavlinkHilSensor','sci_mavlinkHilState','sci_mavlinkHilTracker']';

oooark_sci_gateway_path=get_absolute_file_path('loader.sce');
link(oooark_sci_gateway_path+'../build/src/scicos/liboooarkScicos.so',[jsbsimBlocks;mavlinkBlocks;simgearBlocks;osgBlocks;lapackBlocks]','c');

CurrentDirectory = pwd();
mainpathL = get_absolute_file_path("loader.sce");
chdir(mainpathL);
if isdir("sci_gateway") then
    chdir("sci_gateway");
    exec("loader.sce");
    chdir("..");
end
if isdir("macros") then
    chdir("macros");
    exec("loader.sce");
    chdir("..");
end
if isdir("src") then
    chdir("src");
    exec("loader.sce");
    chdir("..");
end
if isdir("help") then
    chdir("help");
    exec("loader.sce");
    chdir("..");
end
if isdir("scicos") then
    chdir("scicos");
    exec("loader.sce");
    chdir("..");
end
if isdir("demos") then
    chdir("demos");
    exec("loader.sce");
    chdir("..");
end
chdir(CurrentDirectory);
clear mainpathL get_absolute_file_path isdir get_file_path functions CurrentDirectory
