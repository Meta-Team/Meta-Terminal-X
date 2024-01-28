#include <QApplication>
#include <TerminalWindow.hpp>
int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	//int a[4];
	//int *b = new int[4];
	//printf("&a[0]=%p,&a[1]=%p,&a[2]=%p,&a[3]=%p\n&b[0]=%p,&b[1]=%p,&b[2]=%p,&b[3]=%p\n",&a[0],&a[1],&a[2],&a[3],&b[0],&b[1],&b[2],&b[3]);
	//printf("typeof(int*)=%d\n",sizeof(int*));
	TerminalWindow terminal;
	terminal.show();
	return app.exec();
}
