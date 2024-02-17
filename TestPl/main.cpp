#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "a.h"
#include "b.h"
#include "c.h"
#include "d.h"

#include <vector>
using namespace std;
vector<vector<int> > ggl;
int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	qmlRegisterType<A>("test", 1, 0, "A");
	  qmlRegisterType<B>("test", 1, 0, "B");
	  qmlRegisterType<C>("test", 1, 0, "C");
	qmlRegisterType<D>("test", 1, 0, "D");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	return app.exec();
}
