#include <QApplication>
#include <TerminalWindow.hpp>
int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	TerminalWindow terminal;
	terminal.show();
	return app.exec();
}
