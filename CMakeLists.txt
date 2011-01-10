cmake_minimum_required (VERSION 2.6)

project (oooark)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "In-source builds are not allowed. For example run:
	rm CMakeCache.txt
	mkdir build
	cd build
	cmake ..
	make")
endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

enable_language(C CXX Fortran)

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

# find libraries with cmake modules
find_package(Qt4)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTUITOOLS TRUE)
if (QT4_FOUND)
	message(STATUS "Found QT4")
endif(QT4_FOUND)

find_package(Boost 1.40.0 REQUIRED COMPONENTS thread-mt system-mt)
if (Boost_FOUND)
	message(STATUS "Found BOOST")
endif(Boost_FOUND)

find_package(OpenSceneGraph 2.8.3 COMPONENTS osgDB osgUtil osgViewer)
if (OPENSCENEGRAPH_FOUND)
	message(STATUS "Found OPENSCENEGRAPH")
endif(OPENSCENEGRAPH_FOUND)

find_package(LAPACK)
if (LAPACK_FOUND)
	message(STATUS "Found LAPACK")
endif(LAPACK_FOUND)

# find libraries with custom cmake modules
find_package(SIMGEAR)
find_package(SCICOSLAB)

# libraries built from source
set(MAVLINK_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/mavlink/include")
set(BOOST_NUMERIC_BINDINGS_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/boost-numeric-bindings")
set(JSBSIM_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/jsbsim/src")
set(JSBSIM_LIBRARIES "${PROJECT_BINARY_DIR}/jsbsim/src/.libs/libjsbsim.a")

# data directory
if(NOT DEFINED DATADIR)
	set(DATADIR ${PROJECT_SOURCE_DIR}/data)
endif(NOT DEFINED DATADIR)

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

# custom commands for submodules
add_custom_command(OUTPUT  ${PROJECT_BINARY_DIR}/dir.stamp
	COMMAND mkdir ARGS -p jsbsim
	COMMAND touch ARGS ${PROJECT_BINARY_DIR}/dir.stamp
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/git.stamp
	COMMAND git ARGS submodule init
	COMMAND git ARGS submodule update
	COMMAND touch ARGS ${PROJECT_BINARY_DIR}/git.stamp
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/jsbsimAutogen.stamp
	COMMAND ./autogen.sh --no-configure
	COMMAND touch ARGS ${PROJECT_BINARY_DIR}/jsbsimAutogen.stamp
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/thirdParty/jsbsim)

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/jsbsim.stamp
	COMMAND ${PROJECT_SOURCE_DIR}/thirdParty/jsbsim/configure --enable-libraries
	COMMAND make
	COMMAND touch ARGS ${PROJECT_BINARY_DIR}/jsbsim.stamp
	DEPENDS ${PROJECT_BINARY_DIR}/jsbsimAutogen.stamp
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/jsbsim)

add_custom_target(config ALL
	DEPENDS 
	${PROJECT_BINARY_DIR}/dir.stamp 
	${PROJECT_BINARY_DIR}/git.stamp 
	${PROJECT_BINARY_DIR}/jsbsim.stamp)

# internal library list definition / dep summary
message(STATUS "==================================")
message(STATUS "\tLIBRARY\t\tBUILDING")
message(STATUS "==================================")
set(OOOARK_LIBRARIES oooarkCommunication)
message(STATUS "\tCommunication\tYES")

if (SCICOSLAB_FOUND)
	message(STATUS "\tScicos\t\tYES")
else()
	message(STATUS "\tScicos\t\tNO")
endif (SCICOSLAB_FOUND)

if (OPENSCENEGRAPH_FOUND)
	list(APPEND OOOARK_LIBRARIES oooarkVisualization)
	message(STATUS "\tVisualization\tYES")
else()
	message(STATUS "\tVisualization\tNO")
endif (OPENSCENEGRAPH_FOUND)

if (LAPACK_FOUND)
	list(APPEND OOOARK_LIBRARIES oooarkMath)
	message(STATUS "\tMath\t\tYES")
else()
	message(STATUS "\tMath\t\tNO")
endif (LAPACK_FOUND)
message(STATUS "==================================")

# project wide flags
include_directories( ${PROJECT_SOURCE_DIR}/src )

# subdirectories
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(app)
