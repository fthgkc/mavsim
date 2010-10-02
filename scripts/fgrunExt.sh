#!/bin/bash
#--native-ctrls=socket,out,119,,5501,udp \
#--native-ctrls=socket,in,120,,5502,udp \
#--disable-textures \
if [ $# != 1 ]
then
	echo usage: $0 aircraft
	exit
fi
aircraft=$1
fgfs \
--fdm=external \
--aircraft=$aircraft \
--geometry=400x300 \
--native-fdm=socket,in,120,,5500,udp \
--vc=30 \
--altitude=1000 \
--heading=90 \
--roll=0 \
--pitch=0 \
--wind=0@0 \
--turbulence=0.0 \
--timeofday=noon \
--shading-flat \
--notrim \
--fog-disable \
--disable-specular-highlight \
--disable-skyblend \
--disable-random-objects \
--disable-panel \
--disable-horizon-effect \
--disable-clouds \
--disable-anti-alias-hud
