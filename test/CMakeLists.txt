include_directories( "${PROJECT_SOURCE_DIR}/src")
link_directories(${PROJECT_BINARY_DIR})

set(oooark_src
	ins.cpp
	)
add_executable(ins ${oooark_src})
target_link_libraries(ins ${Boost_LIBRARIES} ${OOOARK_LIBRARIES})
