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
		set(${PACKAGE}_FOUND TRUE)
	else()
		add_custom_command(OUTPUT ${PACKAGE}.stamp 
			COMMAND touch ${PACKAGE}.stamp)
	endif()
	add_custom_target(${PACKAGE} DEPENDS ${PACKAGE}.stamp)
endmacro(find_or_build_from_source)

macro(set_default VAR DEFAULT)
	if (NOT DEFINED ${VAR})
		set(${VAR} ${DEFAULT})
	endif()
endmacro(set_default)

# check for out of source build
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "In-source builds are not allowed. For example run:
	rm CMakeCache.txt
	mkdir build
	cd build
	cmake ..
	make")
endif()

# settings
set(oooark_VERSION_MAJOR 0)
set(oooark_VERSION_MINOR 2)
set(oooark_VERSION_PATCH 0)
set(oooark_SOVERSION 0)

set_default(MAVLINK_BUILD_FROM_SOURCE FALSE)
set_default(JSBSIM_BUILD_FROM_SOURCE FALSE)
set_default(BOOSTNUMERIBINDINGS_BUILD_FROM_SOURCE FALSE)
set_default(STATIC_LINKING FALSE)
set_default(IN_SRC_BUILD FALSE)

# built variables
set(oooark_VERSION ${oooark_VERSION_MAJOR}.${oooark_VERSION_MINOR}.${oooark_VERSION_PATCH})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# only find static libraries
if(STATIC_LINKING)
	message(WARNING "static linking is not yet fully functional and will have linking errors")
	if(WIN32)
		set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
	else(WIN32)
		set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
	endif(WIN32)
endif()

# set build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# enable languages
include(${CMAKE_MODULE_PATH}/language_support_v2.cmake) 
enable_language(C)
enable_language(CXX)
workaround_9220(Fortran Fortran_langurage_works)
if (Fortran_langurage_works)
	enable_language(Fortran OPTIONAL)
endif(Fortran_langurage_works)

# initialize variables
set(OOOARK_LIBRARIES oooarkNavigation oooarkCommunication)
set(SCICOSLAB_BLOCKS stdBlocks;jsbsimBlocks;mavlinkBlocks)

# installer
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${oooark_VERSION}")
set(CPACK_GENERATOR "DEB")
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CPACK_SET_DESTDIR TRUE)
set(CPACK_PACKAGE_CONTACT "James Goppert james.goppert@gmail.com")
set(CPACK_PACKAGE_DESCRITION_SUMMARY "
    The Open Object Oriented Autonomous Robotics Kit

    This library is useful for controller design for unmanned
	vehicles. It has a trim gui based on qt that can be used
    to craete a linaer model of JSBSim flight dynamic model.
    This model can then be used in the Scicoslab toolbox
    provided for controller design. A simple bicyle model
    for ground vehicles is also implemented in one of the
    demonstration blocks. Hardware in the loop simulation
    is enabled via the MAVLINK protocol.	
	")
set(CPACK_SOURCE_IGNORE_FILES ${CPACK_SOURCE_IGNORE_FILES}
	/.git/;/build/;~$;.*\\\\.bin$;.*\\\\.swp$)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README")
set(CPACK_PACKAGE_VERSION_MAJOR ${oooark_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${oooark_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${oooark_VERSION_PATCH})
include(CPack)

# add make dist target
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

# find libraries with cmake modules
find_package(Qt4)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTUITOOLS TRUE)
find_package(Boost 1.40.0 REQUIRED COMPONENTS thread-mt system-mt)
find_package(OpenGL)
find_package(OpenSceneGraph 2.8.3 COMPONENTS osgGA osgDB osgUtil osgViewer)
find_package(LAPACK)
find_package(SIMGEAR)
find_package(SCICOSLAB)
find_or_build_from_source(BOOSTNUMERICBINDINGS)
find_or_build_from_source(JSBSIM)
find_or_build_from_source(MAVLINK)

# build libraries from source if not found on system
if(JSBSIM_BUILD_FROM_SOURCE)
	set(JSBSIM_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/jsbsim/src")
	set(JSBSIM_LIBRARIES "${PROJECT_BINARY_DIR}/jsbsim/src/.libs/libjsbsim.a")
	add_custom_command(OUTPUT JSBSIM.stamp 
		COMMAND mkdir -p jsbsim
		COMMAND cd ${PROJECT_SOURCE_DIR}/thirdParty/jsbsim && ./autogen.sh --no-configure
		COMMAND cd jsbsim && ${PROJECT_SOURCE_DIR}/thirdParty/jsbsim/configure --enable-libraries
		COMMAND cd jsbsim && make -j8
		COMMAND touch JSBSIM.stamp
		DEPENDS GIT.stamp)
endif()

if(MAVLINK_BUILD_FROM_SOURCE)
	set(MAVLINK_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/mavlink/include")
	add_custom_command(OUTPUT MAVLINK.stamp 
		COMMAND touch MAVLINK.stamp)
endif()

if(BOOSTNUMERICBINDINGS_BUILD_FROM_SOURCE)
	set(BOOSTNUMERICBINDINGS_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/boost-numeric-bindings")
	add_custom_command(OUTPUT BOOSTNUMERICBINDINGS.stamp 
		COMMAND touch BOOSTNUMERICBINDINGS.stamp)
endif()

# data directory
if(IN_SRC_BUILD)
	message(STATUS "configuring for in source build")
	set(DATADIR ${PROJECT_SOURCE_DIR}/data)
	set(LIBDIR ${CMAKE_SOURCE_DIR}/data)
	set(BINDIR ${CMAKE_BINARY_DIR}/bin)
else()
	message(STATUS "configuring for install build")
	set(DATADIR ${CMAKE_INSTALL_PREFIX}/share/oooark/data)
	set(LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
	set(BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
endif()

# install data files
install(DIRECTORY "${PROJECT_SOURCE_DIR}/data"
	DESTINATION share/oooark
	PATTERN "*.git*" EXCLUDE)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/oooark-toolbox"
	DESTINATION ${SCICOSLAB_CONTRIB_DIR}
	PATTERN "*.git*" EXCLUDE)

# pkgconfig
install(FILES
	oooark.pc
	DESTINATION lib/pkgconfig
	)

# git submodules
if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/.git)
	message(STATUS "git repository detected, will attempt to load submodules")
	add_custom_command(OUTPUT GIT.stamp
		COMMAND cd ${PROJECT_SOURCE_DIR} && git submodule init
		COMMAND cd ${PROJECT_SOURCE_DIR} && git submodule update
		COMMAND touch GIT.stamp)
else()
	execute_process(COMMAND touch GIT.stamp)
endif()


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
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(app)
