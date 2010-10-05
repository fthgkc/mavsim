include( ${QT_USE_FILE} )

set(gui_moc_src
	MainWindow.hpp
	)

set(gui_ui_src
	forms/MainWindow.ui
	)

set(gui_res
	)

qt4_wrap_cpp(gui_moc ${gui_moc_src})
qt4_wrap_ui(gui_ui ${gui_ui_src})
qt4_add_resources(gui_res ${gui_res_src})

include_directories(. ${OSG_INCLUDE_DIRS}
	${APMHSL_INCLUDE_DIRS} ${QT_INCLUDE_DIRS}
	${QT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} 
	${OSGEART_INCLUDE_DIRS}) 

set(gui_src
	QOSGAdapterWidget.cpp
	MapWidget.cpp
	MainWindow.cpp
	MapVehicle.cpp
	main.cpp
	${gui_moc}
	${gui_ui}
	${gui_res}
	)

add_definitions(-DUSE_QT4)
add_executable(gui_test ${gui_src})
target_link_libraries(gui_test ${OSG_LIBRARIES} ${QT_LIBRARIES}
	${Boost_LIBRARIES} ${OOOARK_LIBRARIES} 
	${OSGEARTH_LIBRARIES} ${APMHSL_LIBRARIES})

