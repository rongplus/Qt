#include "mydb.h"

MyDB::MyDB(QObject *parent) : QObject(parent)
{
	tm.start(1000*60);
	connect(&tm,SIGNAL(timeout()), this,SIGNAL(dataChanged()));
}
