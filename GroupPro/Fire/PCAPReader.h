// PCAPReader/PCAPReader.h

#ifndef  __PCAPREADER_H__

#define __PCAPREADER_H__


#include <qrunnable.h>
#include <qobject.h>
#include <qthread.h>
#include "ZBType.h"
#include <QtEndian>
#include <QMutex>
#include <QWaitCondition>
class PCAPReader : public QThread
{
    Q_OBJECT

public:
	PCAPReader(QString filename, QObject *parent) : QThread(parent)
	{
		mFileName = filename; 
		mPause = false;
	};
	
	 void run();

	
public:
	

	void resumeReader()
	{
		sync.lock();
		mPause = false;
		sync.unlock();
		pauseCond.wakeAll();
	}

	void pauseReader()
	{
		sync.lock();
		mPause = true;
		sync.unlock();
	}

	bool isPaused() { return mPause;  };
signals:
    void sendPacket(QByteArray ,  ZBTime );
	void Finished(int);

private:
    
	void readPCAP(QString filename);

	QString mFileName;

	QMutex sync;
	QWaitCondition pauseCond;
	bool mPause;

};


#endif // ! 