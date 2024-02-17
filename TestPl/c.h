#ifndef C_H
#define C_H

#include <QObject>
#include "a.h"


class C : public QObject
{
  Q_OBJECT
  Q_PROPERTY(A* a READ a WRITE setA NOTIFY aChanged)
public:
  explicit C(QObject *parent = 0) ;

  A* a(){return myA;}

  void setA(A *a){
	myA = a;
	aChanged(myA);
  }

signals:
  void aChanged(A*);

private:
  A *myA;
};

#endif // C_H
