include_directories( "${PROJECT_SOURCE_DIR}/src" ${OSGGTK_INCLUDE_DIRS}
	${GTKMM_INCLUDE_DIRS} ${APMHSL_INCLUDE_DIRS})
link_directories(${PROJECT_BINARY_DIR})

add_library(fortranC SHARED hello.f90 world.cpp)
target_link_libraries(fortranC gfortran)

add_executable(fortranC_test fortranC.cpp)
target_link_libraries(fortranC_test fortranC)
