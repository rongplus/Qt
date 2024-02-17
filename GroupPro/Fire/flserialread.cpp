#include "flserialread.h"
#include "qx_xmodem.h"
#include <QDebug>

FLSerialRead::FLSerialRead(QSerialPort* ser,QObject *parent)
	: QThread(parent)
{
	m_serial = ser;
	data_array = nullptr;
}

FLSerialRead::~FLSerialRead()
{

}

void FLSerialRead::run()
{
	unsigned int bufsize = 1024;	
	if(data_array == nullptr)
		return;
	QByteArray arr(*data_array);
	//LOG(INFO) << "XMODEM::xTransmit - size " << ba.size();
	if (data_array->size()%bufsize != 0)
	{
		// Space padding in the buffer		
		const char ctrlz[2] = {CTRLZ,0x00};
		QString pad(ctrlz);
		pad = pad.repeated((bufsize-(arr.size()%bufsize)));
		arr.append(pad.toUtf8());		
		qDebug() << "XMODEM::xTransmit - size with padding" << arr.size();
	}	
	
	XMODEM m(m_serial,this);	

	quint32 packetnum = 0;
	quint8 seqnum = 1;
	quint32 ret = 0;
	quint32 res = 0;
	QByteArray payload, packet;

	while (true)
	{
		payload = arr.mid(packetnum*bufsize,bufsize);
		packet.clear();
		packet.append(QString(STX).toUtf8());
		packet.append(seqnum);
		packet.append(~seqnum);
		packet.append(payload);
		unsigned short ccrc = m.crc16_ccitt((unsigned char*)payload.constData(),bufsize);
		packet.append((unsigned short)(ccrc>>8) & 0xFF);
		packet.append((unsigned short)ccrc & 0xFF);
		m_serial->write(packet);
		emit progress(floor((float)(packetnum*bufsize)/arr.size()*100.0f));
	}

	
}

void FLSerialRead::set_data(QByteArray* data)
{
	data_array = data;
}
