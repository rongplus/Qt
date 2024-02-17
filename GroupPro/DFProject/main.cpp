#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "mydb.h"
#include <QQmlContext>

#include "dbconnect.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;

	MyDB db;
	DBConnect dbconn;
	dbconn.login("t50suse2.datafax.com","datafax","ski2live");
	dbconn.setStudyNumber(253);
	dbconn.getSitesInfoList();
	//qDebug() << dbconn.responseStr;

	engine.rootContext()->setContextProperty("MyDB", &db);
	engine.rootContext()->setContextProperty("DFexploreDB", &dbconn);

	engine.load(QUrl(QLatin1String("qrc:/main.qml")));

	return app.exec();
}
