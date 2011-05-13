include( ${QT_USE_FILE} )
link_directories(${OPENSCENEGRAPH_LIBRARY_DIRS} ${QT_LIBRARY_DIRS}) 

set(mavsim_moc_src
	MainWindow.hpp
	)

set(mavsim_ui_src
	MainWindow.ui
	)

set(mavsim_res
	)

qt4_wrap_cpp(mavsim_moc ${mavsim_moc_src})
qt4_wrap_ui(mavsim_ui ${mavsim_ui_src})
qt4_add_resources(mavsim_res ${mavsim_res_src})

include_directories(. ${OPENSCENEGRAPH_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS} ${JSBSIM_INCLUDE_DIRS}
	${MAVSIM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

set(mavsim_src
	MainWindow.cpp
	main.cpp
	${mavsim_moc}
	${mavsim_ui}
	${mavsim_res}
	)

add_definitions(-DUSE_QT4)
add_executable(mavsimmavsim ${mavsim_src})
add_dependencies(mavsimmavsim JSBSIM)
target_link_libraries(mavsimmavsim ${OSG_LIBRARIES}
	${QT_LIBRARIES} ${JSBSIM_LIBRARIES}
	${MAVSIM_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS mavsimmavsim DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
