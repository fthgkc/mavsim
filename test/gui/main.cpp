#include <QApplication>
#include "MainWindow.hpp"

int main (int argc, char * argv[])
{
	using namespace oooark;
	QApplication app(argc,argv);
	MainWindow w;
	w.show();
	return app.exec();
}
