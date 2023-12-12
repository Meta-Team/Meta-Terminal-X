#include "qmainwindow.h"
#include "qwidget.h"
#include <QApplication>
#include <QWidget>
#include <QtOpenGL/QtOpenGL>
#include <Qtcharts/QtCharts>
#include <ThemeWidget.hpp>
#include <ui_main.h>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	// Create the main window
	QMainWindow window;
	window.setWindowTitle("QT5.15 Demo");
	ThemeWidget *widget = new ThemeWidget();
	window.setCentralWidget(widget);
	window.resize(1280, 800);
	// Display the window
	window.show();
	return app.exec();
}
