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

# find scicos on linux
       set(SCICOSLAB_INCLUDE_DIRS "/usr/lib/scicoslab-gtk-4.4b7/routines")
endif (APPLE)

# libraries built from source
set(MAVLINK_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/mavlink/include")
set(JSBSIM_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/jsbsim/src")

# manual lib definitions
set(LAPACK_LIBRARIES lapack)
if(APPLE)
	set(OOOARK_LIBRARIES oooarkCommunication oooarkVisualization)
else(APPLE)
	set(OOOARK_LIBRARIES oooarkCommunication oooarkVisualization oooarkMath)
endif(APPLE)

# find libraries with cmake module
set(Boost_ADDITIONAL_VERSIONS "1.40.0")
find_package(Boost REQUIRED COMPONENTS thread-mt system-mt)
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTUITOOLS TRUE)

# find libraries with pkg-config
include(LibFindMacros)
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
install(DIRECTORY "${PROJECT_SOURCE_DIR}/scicoslab" DESTINATION "share/oooark/" 
	PATTERN "*.git*" EXCLUDE)

# pkgconfig
install(FILES
	oooark.pc
	DESTINATION lib/pkgconfig
	)

# custom commands
add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/git.stamp
	COMMAND git ARGS submodule init
	COMMAND git ARGS submodule update
	COMMAND touch ARGS ${PROJECT_BINARY_DIR}/git.stamp
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/jsbsim.stamp
	COMMAND ${PROJECT_SOURCE_DIR}/thirdParty/jsbsim/configure
	COMMAND make
	COMMAND touch ARGS ${PROJECT_BINARY_DIR}/jsbsim.stamp
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/thirdParty/jsbsim)

add_custom_target(config ALL
	DEPENDS 
	${PROJECT_BINARY_DIR}/git.stamp 
	${PROJECT_BINARY_DIR}/jsbsim.stamp)

# subdirectories
add_subdirectory(src)
if(NOT APPLE)
	add_subdirectory(test)
endif(NOT APPLE)
