mode(-1);

genlib('lib_oooarkVis',pwd()+'/oooarkVis');
create_palette('oooarkVis');
add_palette('oooark Visualization',pwd()+'/oooarkVis/oooarkVis.cosf');

genlib('lib_oooarkHil',pwd()+'/oooarkHil');
create_palette('oooarkHil');
add_palette('oooark Hardware in the Loop',pwd()+'/oooarkHil/oooarkHil.cosf');

genlib('lib_oooarkDynamics',pwd()+'/oooarkDynamics');
create_palette('oooarkDynamics');
add_palette('oooark Dynamics',pwd()+'/oooarkDynamics/oooarkDynamics.cosf');
