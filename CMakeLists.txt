cmake_minimum_required (VERSION 2.6)

project (oooark)

enable_language(C CXX Fortran)

# The version number.
set(oooark_VERSION_MAJOR 0)
set(oooark_VERSION_MINOR 0)
set(oooark_VERSION_PATCH 0)
set(oooark_SOVERSION 0)

set(oooark_VERISON ${oooark_VERSION_MAJOR}.${oooark_VERSION_MINOR}.${oooark_VERSION_PATCH})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH})

# installer
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_CONTACT "James Goppert james.goppert@gmail.com")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${oooark_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${oooark_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${oooark_VERSION_PATCH}")
include (CPack)

# lib defs
include(LibFindMacros)
set(Boost_ADDITIONAL_VERSIONS "1.40.0")
set(SCICOSLAB_INCLUDE_DIRS "/usr/lib/scicoslab-gtk-4.4b7/routines")
set(JSBSIM_INCLUDE_DIRS "/usr/include/JSBSim")
set(OSGEARTH_LIBRARIES osgEarth osgEarthUtil osgEarthFeatures)
set(APMHSL_LIBRARIES "apmhsl")
set(APMHSL_INCLUDE_DIRS "/usr/include/apmhsl")
set(OOOARK_LIBRARIES oooarkCommunication oooarkVisualization oooarkScicos)

# find libraries
find_package(GTK)
libfind_pkg_check_modules(OSG openscenegraph)
libfind_pkg_check_modules(GTKMM gtkmm-2.4 gdkmm-2.4)
libfind_pkg_check_modules(OSGGTK osgGtkmm-1.0)
find_package(Boost REQUIRED COMPONENTS thread-mt system-mt)
find_package(Qt4 REQUIRED)
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTUITOOLS TRUE)

# install data files
install(DIRECTORY "${PROJECT_SOURCE_DIR}/data" DESTINATION "share/oooark/"
	PATTERN "*.git*" EXCLUDE)

# subdirectories
add_subdirectory(test)
add_subdirectory(src)
