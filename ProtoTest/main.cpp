#include "mainwindow.h"
#include <QApplication>
#include "fljobwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FLJobWindow w;
	w.show();

	return a.exec();
}
