include( ${QT_USE_FILE} )
link_directories(${OPENSCENEGRAPH_LIBRARY_DIRS} ${QT_LIBRARY_DIRS}) 

set(trim_moc_src
	MainWindow.hpp
	)

set(trim_ui_src
	MainWindow.ui
	)

set(trim_res
	)

qt4_wrap_cpp(trim_moc ${trim_moc_src})
qt4_wrap_ui(trim_ui ${trim_ui_src})
qt4_add_resources(trim_res ${trim_res_src})

include_directories(. ${OPENSCENEGRAPH_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS} ${JSBSIM_INCLUDE_DIRS}
	${OOOARK_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

set(trim_src
	MainWindow.cpp
	main.cpp
	${trim_moc}
	${trim_ui}
	${trim_res}
	)

add_definitions(-DUSE_QT4)
add_executable(mavsimTrim ${trim_src})
add_dependencies(mavsimTrim JSBSIM)
target_link_libraries(mavsimTrim ${OSG_LIBRARIES}
	${QT_LIBRARIES} ${JSBSIM_LIBRARIES}
	${OOOARK_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS mavsimTrim DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
