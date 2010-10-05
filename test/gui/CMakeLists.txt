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

include_directories(.)

set(gui_src
	QOSGAdapterWidget.cpp
	MainWindow.cpp
	MapVehicle.cpp
	main.cpp
	${gui_moc}
	${gui_ui}
	${gui_res}
	)

add_definitions(-DUSE_QT4)
add_executable(gui ${gui_src})
target_link_libraries(gui ${OSG_LIBRARIES} ${QT_LIBRARIES} ${Boost_LIBRARIES}
	${OOOARK_LIBRARIES} ${OSGEARTH_LIBRARIES})
