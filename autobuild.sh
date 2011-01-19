#!/bin/bash

PS3='Please enter your choice: '
LIST="in_source_build install_build grab_debian_dependencies remake END"
echo 
echo in_source_build: is used for development and you can start the scicoslab toolbox by typing scicoslab in the oooark source directory
echo install_build: is used for building before final installation to the system.
echo grab_debian_dependencies: installs all the required packages for debian based systems \(ubuntu maverick/ debian squeeze,lenny\)
echo remake: calls make again after project has been configured as install or in source build

echo
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
		sudo apt-get install automake libtool
		sudo apt-get install scicoslab-gtk
		exit 0

	elif [ $OPT = "remake" ] &> /dev/null
	then
		echo you chose to recall make on the previously configured build
		cd build && make
		exit 0

	elif [ $OPT = "END" ] &> /dev/null
	then
		exit 0
	fi
done
