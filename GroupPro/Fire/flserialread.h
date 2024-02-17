#ifndef FLSERIALREAD_H
#define FLSERIALREAD_H

#include <QThread>
#include <QSerialPort>

class FLSerialRead : public QThread
{
	Q_OBJECT

public:
	FLSerialRead(QSerialPort* ser,QObject *parent);
	~FLSerialRead();

	virtual void run();
	void set_data(QByteArray* data);
signals:
	void onDataReady(QByteArray );
	void progress(int);
private:
	QSerialPort *m_serial;
	QByteArray* data_array;
};

#endif // FLSERIALREAD_H
