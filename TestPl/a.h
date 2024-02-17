#ifndef A_H
#define A_H

#include <QObject>

class A : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString test READ test WRITE setTest NOTIFY testChanged)
public:
	explicit A(QObject *parent = 0);

QString test(){return myTest;}

  void setTest(QString t){
	myTest = t;
	testChanged(myTest);
  }

signals:
  void testChanged(QString t);

private:
  QString myTest;

signals:

public slots:
};

#endif // A_H
