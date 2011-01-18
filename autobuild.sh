#!/bin/bash

PS3='Please enter your choice: '
LIST="in_source_build install_build grab_debian_dependencies END"
select OPT in $LIST
do
	if [ $OPT = "in_source_build" ] &> /dev/null
	then
		echo you chose in source build
		rm -rf build
		mkdir build
		cd build
		cmake -DIN_SRC_BUILD:bool=TRUE ..
		make
		exit 0
	elif [ $OPT = "install_build" ] &> /dev/null
	then
		echo you chose install build
		rm -rf build
		mkdir build
		cd build
		cmake ..
		make
		exit 0
	elif [ $OPT = "grab_debian_dependencies" ] &> /dev/null
	then
		echo you chose to install debian dependencies
		sudo apt-get install cmake libqt4-dev simgear-dev liblapack-dev libboost-all-dev libopenscenegraph-dev gfortran
		exit 0

	elif [ $OPT = "END" ] &> /dev/null
	then
		exit 0
	fi
done
