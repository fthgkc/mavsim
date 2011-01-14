cmake_minimum_required (VERSION 2.6)

project (oooark)

# marcos
macro(find_or_build_from_source PACKAGE)
	if (NOT ${PACKAGE}_BUILD_FROM_SOURCE)
		find_package(${PACKAGE})
	endif()

	if (NOT ${PACKAGE}_FOUND)
		set(${PACKAGE}_BUILD_FROM_SOURCE TRUE)
		message(STATUS "could not find package ${PACKAGE}, building from source")
	endif()
endmacro(find_or_build_from_source)

# check for out of source build
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "In-source builds are not allowed. For example run:
	rm CMakeCache.txt
	mkdir build
	cd build
	cmake ..
	make")
endif("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

# set build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# settings
set(oooark_VERSION_MAJOR 0)
set(oooark_VERSION_MINOR 1)
set(oooark_VERSION_PATCH 0)
set(oooark_SOVERSION 0)
set(MAVLINK_BUILD_FROM_SOURCE FALSE)
set(JSBSIM_BUILD_FROM_SOURCE FALSE)
set(BOOSTNUMERIBINDINGS_BUILD_FROM_SOURCE FALSE)

# built variables
set(oooark_VERISON ${oooark_VERSION_MAJOR}.${oooark_VERSION_MINOR}.${oooark_VERSION_PATCH})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH})

# enable languages
include(${CMAKE_MODULE_PATH}/language_support_v2.cmake) 
enable_language(C)
enable_language(CXX)
workaround_9220(Fortran Fortran_langurage_works)
if (Fortran_langurage_works)
	enable_language(Fortran OPTIONAL)
endif(Fortran_langurage_works)

# initialize lists
set(configDepends  
	${PROJECT_BINARY_DIR}/dir.stamp 
	${PROJECT_BINARY_DIR}/git.stamp 
	)
set(OOOARK_LIBRARIES oooarkCommunication)
set(SCICOSLAB_BLOCKS jsbsimBlocks;mavlinkBlocks)

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
find_package(Boost 1.40.0 REQUIRED COMPONENTS thread-mt system-mt)
find_package(OpenSceneGraph 2.0.0 COMPONENTS osgDB osgUtil osgViewer)
find_package(LAPACK)
find_package(SIMGEAR)
find_package(SCICOSLAB)
find_package(JSBSIM)
find_or_build_from_source(BOOSTNUMERICBINDINGS)
find_or_build_from_source(JSBSIM)
find_or_build_from_source(MAVLINK)

# build libraries from source if not found on system
if (MAVLINK_BUILD_FROM_SOURCE)
	set(MAVLINK_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/mavlink/include")
	set(MAVLINK_FOUND TRUE)
endif()

if (BOOSTNUMERICBINDINGS_BUILD_FROM_SOURCE)
	set(BOOSTNUMERICBINDINGS_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/boost-numeric-bindings")
	set(BOOSTNUMERICBINDINGS_FOUND TRUE)
endif()

if (JSBSIM_BUILD_FROM_SOURCE)
	set(JSBSIM_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/jsbsim/src")
	set(JSBSIM_LIBRARIES "${PROJECT_BINARY_DIR}/jsbsim/src/.libs/libjsbsim.a")
	list(APPEND configDepends ${PROJECT_BINARY_DIR}/jsbsim.stamp)
	set(JSBSIM_FOUND TRUE)
endif()

# data directory
if(NOT DEFINED DATADIR)
	set(DATADIR ${PROJECT_SOURCE_DIR}/data)
endif()

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

# config dependencies
add_custom_target(config ALL DEPENDS ${configDepends}) 

# internal library list definition / dep summary
message(STATUS "=======================================")
message(STATUS "\tLIBRARY\t\t\tBUILDING")
message(STATUS "=======================================")
message(STATUS "\tCommunication\t\tYES")
message(STATUS "\t\tMavlink\t\tYES")
message(STATUS "\t\tJSBSim\t\tYES")
if (SIMGEAR_FOUND)
	list(APPEND SCICOSLAB_BLOCKS simgearBlocks)
	message(STATUS "\t\tSimgear\t\tYES")
else()
	message(STATUS "\t\tSimgear\t\tNO")
endif (SIMGEAR_FOUND)

if (SCICOSLAB_FOUND)
	message(STATUS "\tScicos\t\t\tYES")
else()
	message(STATUS "\tScicos\t\t\tNO")
endif (SCICOSLAB_FOUND)

if (OPENSCENEGRAPH_FOUND)
	list(APPEND OOOARK_LIBRARIES oooarkVisualization)
	list(APPEND SCICOSLAB_BLOCKS osgBlocks)
	message(STATUS "\tVisualization\t\tYES")
	message(STATUS "\t\tOpenSceneGraph\tYES")
else()
	message(STATUS "\tVisualization\t\tNO")
	message(STATUS "\t\tOpenSceneGraph\t\tNO")
endif (OPENSCENEGRAPH_FOUND)
if (QT4_FOUND)
	message(STATUS "\t\tQT4\t\tYES")
else()
	message(STATUS "\t\tQT4\t\tNO")
endif (QT4_FOUND)

if (LAPACK_FOUND)
	list(APPEND OOOARK_LIBRARIES oooarkMath)
	list(APPEND SCICOSLAB_BLOCKS lapackBlocks)
	message(STATUS "\tMath\t\t\tYES")
	message(STATUS "\t\tLapack\t\tYES")
else()
	message(STATUS "\tMath\t\t\tNO")
	message(STATUS "\t\tLapack\t\tNO")
endif (LAPACK_FOUND)
if (CMAKE_Fortran_COMPILER_WORKS)
	message(STATUS "\t\tFortran\t\tYES")
else()
	message(STATUS "\t\tFortran\t\tNO")
	set(CMAKE_Fortran_COMPILER none)
endif (CMAKE_Fortran_COMPILER_WORKS)



message(STATUS "=======================================")

# project wide flags
include_directories( ${PROJECT_SOURCE_DIR}/src )

# subdirectories
add_subdirectory(config)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(app)
