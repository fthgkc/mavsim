cmake_minimum_required (VERSION 2.6)

project (oooark)

#enable_language(C CXX Fortran)

# The version number.
set(oooark_VERSION_MAJOR 0)
set(oooark_VERSION_MINOR 0)
set(oooark_VERSION_PATCH 0)
set(oooark_SOVERSION 0)

set(oooark_VERISON ${oooark_VERSION_MAJOR}.${oooark_VERSION_MINOR}.${oooark_VERSION_PATCH})

# installer
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_CONTACT "James Goppert james.goppert@gmail.com")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${oooark_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${oooark_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${oooark_VERSION_PATCH}")
include (CPack)

# boost
set(Boost_ADDITIONAL_VERSIONS "1.40.0")
find_package(Boost REQUIRED COMPONENTS thread-mt system-mt)
link_directories( ${Boost_LIBRARIES_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )

# subdirectories
add_subdirectory(src)
add_subdirectory(test)
