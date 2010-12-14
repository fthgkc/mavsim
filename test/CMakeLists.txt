include_directories( "${PROJECT_SOURCE_DIR}/src")
link_directories(${PROJECT_BINARY_DIR})
link_libraries(${Boost_LIBRARIES} ${OOOARK_LIBRARIES})

add_executable(ins ins.cpp)
add_executable(visualization visualization.cpp)
