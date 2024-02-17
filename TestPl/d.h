#ifndef D_H
#define D_H

#include <QObject>
#include <QDebug>
#include <QObject>
#include <QVariantList>

class D : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QVariantList vl READ vl WRITE setVl NOTIFY vlChanged)

public:
  explicit D(QObject *parent = 0) ;

  QVariantList vl(){return myVl;}

  void setVl(QVariantList vl){
	myVl = vl;
	vlChanged(myVl);
  }

  Q_INVOKABLE void doVl(){
	qDebug() << "Size of myVl: " << myVl.size();
	for(int i = 0; i < myVl.size(); i++){
	  qDebug() << "myVl.at(" << i << "): " << myVl.at(i);
	}
  }

signals:
  void vlChanged(QVariantList);

private:
  QVariantList myVl;

};

#endif // D_H
