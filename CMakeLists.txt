cmake_minimum_required (VERSION 2.6)

project (oooark)

#enable_language(C CXX Fortran)

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
set(Boost_ADDITIONAL_VERSIONS "1.40.0")
set(SCICOSLAB_INCLUDE_DIR "/usr/lib/scicoslab-gtk-4.4b7/routines")
set(JSBSIM_INCLUDE_DIR "/usr/include/JSBSim")

# find libraries
find_package(Boost REQUIRED COMPONENTS thread-mt system-mt)
find_package(OSG)

# subdirectories
add_subdirectory(src)
add_subdirectory(test)
