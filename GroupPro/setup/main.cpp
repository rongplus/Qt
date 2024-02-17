#include "mainwindow.h"
#include <QApplication>
#include "df.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DF w;
	w.show();

	return a.exec();
}
