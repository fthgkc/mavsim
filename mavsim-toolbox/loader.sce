mode(-1);
lines(0);

mavsimPath=get_absolute_file_path('loader.sce');

// disable white color plot, because you can't see it with a white background
// TODO: get this pushed into scicoslab
f = gdf();
f.color_map(8,:) = [0,0,0]; // set white to black in color map so it can be seen

chdir(mavsimPath);

mprintf('mavsim version %s\n', stripblanks(read("VERSION",1,1,'(a)')) );
mprintf('Copyright (C) 2010 James Goppert\n\n' );

if isdir('sci_gateway') then
  chdir('sci_gateway');
  exec('loader.sce');
  chdir('..');
end

if isdir('macros') then
  chdir('macros');
  exec('loadmacros.sce');
  chdir('..');
end

if isdir('scicos') then
  chdir('scicos');
  exec('loadscicos.sce');
  chdir('..');
end

if isdir('demos') then
  chdir('demos');
  exec('loaddemos.sce');
  chdir('..');
end

if isdir('help') then
  chdir('help');
  exec('loadhelp.sce');
  chdir('..');
end
