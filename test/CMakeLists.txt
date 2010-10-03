include_directories( "${PROJECT_SOURCE_DIR}/src" ${OSGGTK_INCLUDE_DIRS}
	${GTKMM_INCLUDE_DIRS} /usr/include/apmhsl)
link_directories(${PROJECT_BINARY_DIR})

add_library(fortranC SHARED hello.f90 world.cpp)
target_link_libraries(fortranC gfortran)

add_executable(fortranC_test fortranC.cpp)
target_link_libraries(fortranC_test fortranC)

add_executable(gcs_test gcs.cpp)
target_link_libraries(gcs_test oooarkCommunication osggtk ${GTKMM_LIBRARIES}) 
