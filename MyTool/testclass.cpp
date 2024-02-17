#include "testclass.h"
#include <QScriptEngine>
#include <QDebug>
#include <QFile>

void testScript()
{
	QScriptEngine myEngine;
	myEngine.evaluate("function cube(x) { return x * x * x; }    function mysqrt(x) { return Math.sqrt(x);} ");
	qDebug()<< myEngine.evaluate("mysqrt(cube(3))").toNumber();
}

TestClass::TestClass()
{
		QString result = "";
		QFile scriptFile("/Users/rong/Desktop/projects/Mac_Qt/MyTool/fun.js");
		if (!scriptFile.open(QIODevice::ReadOnly))
		{
			result.clear();
			qWarning() << "encodePass.js open failed";
			return;
		}
		QTextStream out(&scriptFile);
		QString contents = out.readAll();
		scriptFile.close();

		double var1 = 3.0;
		double var2 = 4.0;
		qDebug() << "var1:" << var1 << "var2:" << var2;

		QScriptValueList args;      //调用js方法时传入的参数
		args << QScriptValue(var1) << QScriptValue(var2);
		QScriptEngine engine;
		QScriptValue js = engine.evaluate(contents);        //个人理解：加载js文本到操作引擎
		QScriptValue func;
		func = engine.globalObject().property("trans2wCal");   //调用js方法
		result = func.call(QScriptValue(), args).toString();
		qDebug() << "result:" << result;

		func = engine.globalObject().property("trans3wCal");   //调用js方法
		result = func.call(QScriptValue(), args).toString();
		qDebug() << "result:" << result;

}
