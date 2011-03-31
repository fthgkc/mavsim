project (mavsim)

# Requried cmake version
cmake_minimum_required (VERSION 2.6)

# global needed variables
set(APPLICATION_NAME ${PROJECT_NAME})

set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "1")
set(APPLICATION_VERSION_PATCH "0")

set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")
set(LIBRARY_VERSION ${APPLICATION_VERSION})

# SOVERSION scheme: CURRENT.AGE.REVISION
#   If there was an incompatible interface change:
#     Increment CURRENT. Set AGE and REVISION to 0
#   If there was a compatible interface change:
#     Increment AGE. Set REVISION to 0
#   If the source code was changed, but there were no interface changes:
#     Increment REVISION.
set(LIBRARY_SOVERSION "1.0.0")

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake/Modules
)

# add definitions
include(DefineCMakeDefaults)
include(DefinePlatformDefaults)
include(DefineCompilerFlags)
include(DefineInstallationPaths)
include(DefineOptions.cmake)
include(CPackConfig.cmake)

# disallow in-source build
include(MacroEnsureOutOfSourceBuild)
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build. 
Please create a separate build directory and run 'cmake /path/to/${PROJECT_NAME} [options]' there.")

# add macros
include(MacroAddPlugin)
include(MacroCopyFile)
include(MacroFindOrBuild)
include(MacroSetDefault)
include(language_support_v2)
include(MacroAddLinkFlags)

# settings
MacroSetDefault(MAVLINK_BUILD_FROM_SOURCE FALSE)
MacroSetDefault(JSBSIM_BUILD_FROM_SOURCE FALSE)
MacroSetDefault(BOOSTNUMERIBINDINGS_BUILD_FROM_SOURCE FALSE)
MacroSetDefault(STATIC_LINKING FALSE)
MacroSetDefault(IN_SRC_BUILD FALSE)

# built variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# only find static libraries
#if(STATIC_LINKING)
	#message(WARNING "static linking is not yet fully functional and will have linking errors")
	#if(WIN32)
		#set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
	#else(WIN32)
		#set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
	#endif(WIN32)
#endif()

# set build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# enable languages
enable_language(C)
enable_language(CXX)
workaround_9220(Fortran Fortran_langurage_works)
if (Fortran_langurage_works)
	enable_language(Fortran OPTIONAL)
endif(Fortran_langurage_works)

# initialize variables
set(MAVSIM_LIBRARIES mavsimNavigation mavsimCommunication)
set(SCICOSLAB_BLOCKS stdBlocks;jsbsimBlocks;mavlinkBlocks)

# add make dist target
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

# git submodules
if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/.git)
	message(STATUS "git repository detected, will attempt to load submodules")
	set(FOUND_GIT_REPO TRUE)
	add_custom_command(OUTPUT GIT.stamp 
		COMMAND cd ${PROJECT_SOURCE_DIR} && git submodule init ${GIT_SUBMODULES}
		COMMAND cd ${PROJECT_SOURCE_DIR} && git submodule update ${GIT_SUBMODULES})
	add_custom_target(GIT DEPENDS GIT.stamp)
else()
	set(FOUND_GIT_REPO FALSE)
endif()

# find libraries with cmake modules
find_package(Qt4)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTUITOOLS TRUE)
find_package(Boost 1.40.0 REQUIRED COMPONENTS thread-mt system-mt)
find_package(OpenGL)
find_package(OpenSceneGraph 2.8.0 COMPONENTS osgGA osgDB osgUtil osgViewer)
if ("OSG_LIBRARY-NOTFOUND" STREQUAL "${OSG_LIBRARY}")
    set(OPENSCENEGRAPH_FOUND FALSE)
else()
    set(OPENSCENEGRAPH_FOUND TRUE)
endif()
find_package(LAPACK)
find_package(SIMGEAR)
find_package(PLIB)
find_package(SCICOSLAB)
MacroFindOrBuild(BOOSTNUMERICBINDINGS thirdParty/boost-numeric-bindings FOUND_GIT_REPO)
MacroFindOrBuild(JSBSIM thirdParty/jsbsim FOUND_GIT_REPO)
MacroFindOrBuild(MAVLINK thirdParty/mavlink FOUND_GIT_REPO)
MacroFindOrBuild(EASYSTAR data/easystar FOUND_GIT_REPO)

# build libraries from source if not found on system
if(JSBSIM_BUILD_FROM_SOURCE)
	set(JSBSIM_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/jsbsim/src")
	set(JSBSIM_LIBRARIES "${PROJECT_BINARY_DIR}/jsbsim/src/.libs/libjsbsim.a")
	add_custom_command(OUTPUT JSBSIM_BUILD.stamp 
		COMMAND mkdir -p jsbsim
		COMMAND cd ${PROJECT_SOURCE_DIR}/thirdParty/jsbsim && ./autogen.sh --no-configure
		COMMAND cd jsbsim && ${PROJECT_SOURCE_DIR}/thirdParty/jsbsim/configure --enable-libraries --with-pic
		COMMAND cd jsbsim && make -j8
		COMMAND touch JSBSIM_BUILD.stamp)
endif()

if(MAVLINK_BUILD_FROM_SOURCE)
	set(MAVLINK_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/mavlink/include")
	add_custom_command(OUTPUT MAVLINK_BUILD.stamp 
		COMMAND touch MAVLINK_BUILD.stamp)
endif()

if(BOOSTNUMERICBINDINGS_BUILD_FROM_SOURCE)
	set(BOOSTNUMERICBINDINGS_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/thirdParty/boost-numeric-bindings")
	add_custom_command(OUTPUT BOOSTNUMERICBINDINGS_BUILD.stamp 
		COMMAND touch BOOSTNUMERICBINDINGS_BUILD.stamp)
endif()

if(EASYSTAR_BUILD_FROM_SOURCE)
	set(EASYSTAR_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/data/easystar")
	add_custom_command(OUTPUT EASYSTAR_BUILD.stamp 
		COMMAND touch EASYSTAR_BUILD.stamp)
endif()

# data directory
if(IN_SRC_BUILD)
	message(STATUS "configuring for in source build")
	set(DATADIR ${PROJECT_SOURCE_DIR}/data)
	set(LIBDIR ${CMAKE_BINARY_DIR}/bin)
	set(BINDIR ${CMAKE_BINARY_DIR}/bin)
else()
	message(STATUS "configuring for install build")
	set(DATADIR ${CMAKE_INSTALL_PREFIX}/share/mavsim/data)
	set(LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
	set(BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
endif()

# install data files
install(DIRECTORY "${PROJECT_SOURCE_DIR}/data"
	DESTINATION share/mavsim
	PATTERN "*.git*" EXCLUDE)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/mavsim-toolbox"
	DESTINATION ${SCICOSLAB_CONTRIB_DIR}
	PATTERN "*.git*" EXCLUDE)

# pkgconfig
install(FILES
	mavsim.pc
	DESTINATION lib/pkgconfig
	)

#
# internal library list definition / dep summary
message(STATUS "=================================")
message(STATUS "\tLIBRARY\t\tBUILDING")
message(STATUS "=================================")
message(STATUS "\tMavlink\t\t${MAVLINK_FOUND}")
message(STATUS "\tJSBSim\t\t${JSBSIM_FOUND}")
message(STATUS "\tPLIB\t\t${PLIB_FOUND}")

message(STATUS "\tSimgear\t\t${SIMGEAR_FOUND}")
if (SIMGEAR_FOUND)
	list(APPEND SCICOSLAB_BLOCKS simgearBlocks)
endif()

message(STATUS "\tScicoslab\t${SCICOSLAB_FOUND}")
if (SCICOSLAB_FOUND)
endif()

message(STATUS "\tOpenSceneGraph\t${OPENSCENEGRAPH_FOUND}")
if (OPENSCENEGRAPH_FOUND)
	list(APPEND MAVSIM_LIBRARIES mavsimVisualization)
	list(APPEND SCICOSLAB_BLOCKS osgBlocks)
endif()

message(STATUS "\tQT4\t\t${QT4_FOUND}")
if (QT4_FOUND)
endif ()

message(STATUS "\tBoost\t\t${Boost_FOUND}")
if (Boost_FOUND)
endif ()

message(STATUS "\tLapack\t\t${LAPACK_FOUND}")
if (LAPACK_FOUND)
	list(APPEND MAVSIM_LIBRARIES mavsimMath)
	list(APPEND SCICOSLAB_BLOCKS lapackBlocks)
endif ()

if (CMAKE_Fortran_COMPILER_WORKS)
	set(FORTRAN_FOUND TRUE)
	set(CMAKE_Fortran_COMPILER none)
else()
	set(FORTRAN_FOUND FALSE)
endif ()

message(STATUS "\tFortran\t\t${FORTRAN_FOUND}")

message(STATUS "=================================")

# project wide flags
include_directories( ${PROJECT_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src)

# subdirectories
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(app)
