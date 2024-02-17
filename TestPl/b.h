#ifndef B_H
#define B_H

#include <QObject>

#include "a.h"
#include "c.h"

#include <QDebug>

class B : public QObject
{
  Q_OBJECT
public:
  explicit B(QObject *parent = 0) ;

  Q_INVOKABLE void doA(A *a){
	qDebug() << "doing A: " << a->test();
  }

  Q_INVOKABLE void doC(C *c){
	qDebug() << "doing C: " << c->a()->test();
  }

  Q_INVOKABLE void doAlist(QVariantList vl){
	qDebug() << "Doing AList... ";
	for(int i = 0; i < vl.size(); i++){
      //qDebug() << "vl.at(" << i << "): " << vl.at(i);
	  // Get the actual object out of a QVariant.
//	  A *a = qobject_cast<A *>(vl.at(i).value<QObject *>());
//	  qDebug() << "Data from A" << i << ": " << a->test();
	}
  }

  Q_INVOKABLE void doVl(QVariantList vl){
	qDebug() << "QVariantList passed as parameter.\nSize of vl: " << vl.size();
	for(int i = 0; i < vl.size(); i++){
//	  qDebug() << "vl.at(" << i << "): " << vl.at(i);
	}
  }

  Q_INVOKABLE A* makeA(){
	A* a = new A();
	a->setTest("Another A");
	return a;
  }

  Q_INVOKABLE QVariantList makeAList(){
	QVariantList vl;

	A* a1 = new A();
	a1->setTest("newA1");
	vl.append(qVariantFromValue((QObject*)a1));

	A* a2 = new A();
	a2->setTest("newA2");
	vl.append(qVariantFromValue((QObject*)a2));

	return vl;
  }
};

#endif // B_H
