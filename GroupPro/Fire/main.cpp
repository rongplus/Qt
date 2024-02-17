#include "firelink.h"
#include <QtWidgets/QApplication>
#include <QSplashScreen>

class I : public QThread
{
public:
    static void sleep(unsigned long secs) { QThread::sleep(secs); }
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/firelink/Resources/icons/sys-stat-icon-48.png"));
	FireLink w;
/*
	//QPixmap pixmap(":/firelink/Resources/graphics/splash.png");
	//QSplashScreen splash(pixmap.scaled(300, 400));
    //splash.show();
    
	//I::sleep(2); // splash is shown for 5 seconds


	

	//splash.finish(&w);

*/
	w.show();
	return a.exec();
}
