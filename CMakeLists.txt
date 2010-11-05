cmake_minimum_required (VERSION 2.6)

project (oooark)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

enable_language(C CXX)

# The version number.
set(oooark_VERSION_MAJOR 0)
set(oooark_VERSION_MINOR 1)
set(oooark_VERSION_PATCH 0)
set(oooark_SOVERSION 0)

set(oooark_VERISON ${oooark_VERSION_MAJOR}.${oooark_VERSION_MINOR}.${oooark_VERSION_PATCH})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH})

# installer
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_CONTACT "James Goppert james.goppert@gmail.com")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set (CPACK_PACKAGE_VERSION_MAJOR "${oooark_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${oooark_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${oooark_VERSION_PATCH}")
include (CPack)

# find scicos on Mac OS X
if (APPLE)
       execute_process(COMMAND mdfind "kMDItemKind == Application && kMDItemDisplayName == ScicosLabGtk"
		   			   COMMAND head -1
                       RESULT_VARIABLE RESULT
                       OUTPUT_VARIABLE SCICOS_APP_BUNDLE
                       ERROR_VARIABLE ERROR_MESSAGE
                       OUTPUT_STRIP_TRAILING_WHITESPACE)
       if (RESULT) 
               MESSAGE(FATAL_ERROR "Could not locate ScicosLabGtk.app - ${ERROR_MESSAGE}")
       endif (RESULT)
       execute_process(COMMAND find ${SCICOS_APP_BUNDLE} -name routines
		   			   COMMAND head -1
                       RESULT_VARIABLE RESULT
                       OUTPUT_VARIABLE SCICOSLAB_INCLUDE_DIRS
                       ERROR_VARIABLE ERROR_MESSAGE
                       OUTPUT_STRIP_TRAILING_WHITESPACE)
       if (RESULT) 
               MESSAGE(FATAL_ERROR "Could not locate scicos_block4.h in ScicosLabGtk.app - ${ERROR_MESSAGE}")
       endif (RESULT)  
       MESSAGE(STATUS "Found ScicosLab headers in ${SCICOSLAB_INCLUDE_DIRS}")
else (APPLE)
       set(SCICOSLAB_INCLUDE_DIRS "/usr/lib/scicoslab-gtk-4.4b7/routines")
endif (APPLE)

# manual lib definitions
set(OSGEARTH_LIBRARIES osgEarth osgEarthUtil osgEarthFeatures)
#set(SIMGEAR_LIBRARIES sgio)
set(OOOARK_LIBRARIES oooarkCommunication oooarkVisualization oooarkScicos)

# find libraries with cmake module
set(Boost_ADDITIONAL_VERSIONS "1.40.0")
find_package(Boost REQUIRED COMPONENTS thread-mt system-mt)
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTUITOOLS TRUE)

# find libraries with pkg-config
include(LibFindMacros)
libfind_pkg_check_modules(JSBSIM jsbsim>=1.0.rc3-advtrim)
libfind_pkg_check_modules(APM ardupilotmegacomm>=0.1.0)
libfind_pkg_check_modules(OSG openscenegraph>=2.8)

# data directory
if(NOT DEFINED DATADIR)
	set(DATADIR ${PROJECT_SOURCE_DIR}/data)
endif(NOT DEFINED DATADIR)
add_definitions(-DDATADIR="${DATADIR}")

# the install_name directory component to be used when installing
set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
message(STATUS "install_name ${CMAKE_INSTALL_NAME_DIR}")

# install data files
install(DIRECTORY "${PROJECT_SOURCE_DIR}/data" DESTINATION "share/oooark/" 
	PATTERN "*.git*" EXCLUDE)

# pkgconfig
install(FILES
	oooark.pc
	DESTINATION lib/pkgconfig
	)

# subdirectories
add_subdirectory(src)
