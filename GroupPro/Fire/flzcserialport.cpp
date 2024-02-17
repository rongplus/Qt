#include "flzcserialport.h"
#include "qx_xmodem.h"
#include "flglobal.h"
#include <QDebug>
#include <QFile>
#define OTA_CONFIG 0xC0

FLZCSerialPort::FLZCSerialPort(QObject *parent)
	: QObject(parent)
{
	_port = nullptr;
	_label = nullptr;
}

FLZCSerialPort::~FLZCSerialPort()
{

}

void FLZCSerialPort::set_port(QSerialPort * port)
{
	Q_ASSERT(port != nullptr);
	_port = port;
}
void FLZCSerialPort::set_peremeter(QMap<QString, int>)
{
}
int FLZCSerialPort::send_binary_1K(QString cmd, QByteArray & data, QSerialPort * port)
{
	if (send_command(cmd, port,true) != 0)
		return Write_OutTime;

	XMODEM md(port, 0);
	md.format_data(data);
	QByteArray  start;
	start.append('j');
	//md.xTransmit(data, start);
	
	if (send_data_1K(port, data) == false)
		return Write_OutTime;
		
	return Successed;

}
int FLZCSerialPort::send_binary_80(QString cmd, QByteArray & data, QSerialPort * port)
{	
	if(send_command(cmd, port,true) != 0)
		return Write_OutTime;

	XMODEM md(port, 0);
	md.format_data(data);
	QByteArray  start;
	start.append('j');
	//md.xTransmit(data, start);
	
	if (send_data_80(port, data) == false)
		return Write_OutTime;
	
	return 0;
}
int FLZCSerialPort::send_binary_1K(QString cmd, QString file, QSerialPort * port)
{
	Q_ASSERT(file != "");
	QFile fi(file);
	if(fi.open(QIODevice::ReadOnly) == false)
	{ 
		return -1;
	}
	auto arr = fi.readAll();
	return send_binary_1K(cmd, arr, port);

}
int FLZCSerialPort::send_binary_80(QString cmd, QString file, QSerialPort * port)
{
	return 0;
}
int FLZCSerialPort::send_data(QString cmd, QByteArray & data, QSerialPort * port)
{
	if (send_command(cmd, port,true) != 0)
		return 2;

	port->write(data);

	return 0;
}
int FLZCSerialPort::send_command(QString cmd, QSerialPort * port,  bool bWait_c)
{
	Q_ASSERT(port != nullptr);

	QByteArray out_arr( cmd.toStdString().data(),cmd.length());
	out_arr.append('\r');
	port->write(out_arr);
	qDebug() << "write command == " << out_arr;
	if (port->waitForBytesWritten(5000) == false)
		return Write_OutTime;
	if (port->waitForReadyRead(5000) == false)
		return Write_OutTime;
	auto ret = port->readAll();
	int timers = 20;
	qDebug() << "send_command(0) recv = " << ret;

	ret.replace("\r\n", " ");
	char ch1 = 0x00;
	ret.replace(ch1, " ");

	qDebug() << "send_command(1) recv = " << ret;

	while ((ret.contains("OK") == false && timers >0) && ret.contains("C") == false)
	{
		timers--;
		port->write(out_arr);
		if (port->waitForBytesWritten(1000) == false)
			continue;
		//recv
		if (port->waitForReadyRead(1000) == false)
			continue;
		ret = port->readAll();
	}
	if (timers <= 0)
		return Write_OutTime;
	if (bWait_c == false)
		return 0;

	//wait for c:
	timers = 20;
	if (port->waitForReadyRead(2000) == false)
		return Write_OutTime;
	ret = port->readAll();
	
	while (ret.contains("C") == false && timers >0)
	{
		timers--;
		port->write(out_arr);
		if (port->waitForBytesWritten(500) == false)
			continue;
		//recv
		if (port->waitForReadyRead(500) == false)
			continue;
		ret = port->readAll();
	}
	if (timers <= 0)
		return Write_OutTime;
	return 0;
}
void FLZCSerialPort::show_message(QByteArray arr)
{
	qDebug() << arr;
	if (_label)
		_label->setText(arr);
}

bool FLZCSerialPort::login(QSerialPort * port,QString password)
{
	Q_ASSERT(port != nullptr);
	QByteArray out_arr;
	int nTimes = 20;
	while (nTimes>0)
	{
		QByteArray arr;// = port->readAll();

		//port->write("\r");
		//port->waitForBytesWritten(5000);
		port->waitForReadyRead(5000);
		arr = port->readAll();
		out_arr.append("login");
		out_arr.append('\r');

		port->write(out_arr);
		if (port->waitForBytesWritten(2000) == false)
			continue;
		if (port->waitForReadyRead(2000) == false)
			continue;
		arr = port->readAll();
		qDebug() << "Read Password== " << arr;
		if (arr.contains("Password") == true)
		{		
			out_arr.clear();
			out_arr.append(password);
			out_arr.append('\r');
			port->write(out_arr);

			if (port->waitForBytesWritten(2000) == false)
				continue;
			if (port->waitForReadyRead(2000) == false)
				continue;
			arr = port->readAll();
			qDebug() << "Read Success== " << arr;
			if (arr.contains("Success") == true)
				return true;
		}

		nTimes--;
	}
	
	return false;
}

FLZCSerialPort::TransitState FLZCSerialPort::send_config(QSerialPort * port, QByteArray & data,int package_len)
{
	Q_ASSERT(port != nullptr);
	//if(login(port, password) == false)
	//	return Successed;
	QByteArray out_arr("send config");
	out_arr.append('\r');
	port->write(out_arr);

	if (port->waitForBytesWritten(5000) == false)
		return Write_OutTime;
	if (port->waitForReadyRead(5000) == false)
		return Write_OutTime;
	auto ret = port->readAll();
	int timers = 20;
	while ((ret.contains("OK") == false && timers >0) && ret.contains("C") == false)
	{
		timers--;
		port->write(out_arr);
		if (port->waitForBytesWritten(1000) == false)
			continue;
		//recv
		if (port->waitForReadyRead(1000) == false)
			continue;
		ret = port->readAll();
	}
	if (timers <= 0)
		return Write_OutTime;
	//wait for c:
	timers = 20;
	if (port->waitForReadyRead(2000) == false)
		return Write_OutTime;
	ret = port->readAll();

	while (ret.contains("C") == false && timers >0)
	{
		timers--;
		port->write(out_arr);
		if (port->waitForBytesWritten(500) == false)
			continue;
		//recv
		if (port->waitForReadyRead(500) == false)
			continue;
		ret = port->readAll();
	}
	if (timers <= 0)
		return Write_OutTime;

	XMODEM md(port, 0);
	md.format_data(data);
	QByteArray  start;
	start.append('j');
	//md.xTransmit(data, start);
	//
	if (package_len == 80)
	{
		if (send_data_80(port, data) == false)
			return Write_OutTime;
	}
	else
	{
		if (send_data_1K(port, data) == false)
			return Write_OutTime;
	}
	

	return Successed;
}

bool FLZCSerialPort::send_data_1K(QSerialPort * port, QByteArray & ba)
{	
	unsigned int bufsize = 1024;
	quint32 packetnum = 0;
	quint8 seqnum = 1;
	
	QByteArray payload, packet;
	XMODEM dm(0, 0);
					
	QByteArray ret;//ack
	

	qDebug() << ba;
	while (true)
	{	
		if (packetnum*bufsize + 1 >= ba.size())
		{
			emit progress(100);
			break;
		}

		payload = ba.mid(packetnum*bufsize, bufsize);
		packet.clear();
		packet.append(QString(STX).toUtf8());
		packet.append(seqnum);
		packet.append(~seqnum);
		packet.append(payload);
		unsigned short ccrc = dm.crc16_ccitt((unsigned char*)payload.constData(), bufsize);
		packet.append((unsigned short)(ccrc >> 8) & 0xFF);
		packet.append((unsigned short)ccrc & 0xFF);
		int retry = 0;
		while (true)
		{
			qDebug() << "send package" << packet;
			port->write(packet);
			if (port->waitForBytesWritten(5000) == false)
				return false;
			//recv
			if (port->waitForReadyRead(5000) == false)
				return false;
			ret = port->readAll();//ack
			qDebug() << "Recv package:" << ret;
			show_message(ret);
			char res = ret[0];

			bool bAck = false;
			switch (res)
			{
				case ACK:				
					packetnum++;
					seqnum++;
					bAck = true;
					break;
				case CAN:					
					break;
				case NAK:
					break;
				case 'c':
				case 'C':				
					continue;
				case '>':					
					break;
				default:				
					break;
			}
			
			retry++;
			if (retry>9)
			{	
				return false;
			}
			if (bAck)
				break;
		}
		emit progress(floor((float)(packetnum*bufsize) / ba.size()*100.0f));
		
	}

	
	QByteArray o_arr;
	o_arr.append(EOT);
	port->write(o_arr);

	if (port->waitForReadyRead(5000) == false)
		return false;
	ret = port->readAll();//ack
	qDebug() << "Recv packet 1 :" << ret;

	if (port->waitForReadyRead(5000) == false)
		return false;
	ret = port->readAll();//ack
	qDebug() << "Recv packet 2 :" << ret;
	ret = ret.replace("\r\n", " ");
	char ch1 = 0x00;
	ret = ret.replace(ch1, " ");
	if (ret.contains("OK"))
		return true;
	
	return false;
}

bool FLZCSerialPort::send_data_80(QSerialPort * port, QByteArray & ba)
{
	unsigned int bufsize = 80;
	quint32 packetnum = 0;
	quint8 seqnum = 1;

	QByteArray payload;
	XMODEM dm(0, 0);

	QByteArray ret;//ack


	qDebug() << ba;
	while (true)
	{
		if (packetnum*bufsize + 1 >= ba.size())
		{
			emit progress(100);
			break;
		}

		payload = ba.mid(packetnum*bufsize, bufsize);
		unsigned char Header[] = { 0x02, 0x00, 0x00, 0x52, 0x00, 0x06, 0x06, 0x00, 0x01 };
		QByteArray packet((char *)Header,9);
		packet.append(OTA_CONFIG);
		packet.append(seqnum);
		//packet.append(~seqnum);
		packet.append(payload);
		unsigned short ccrc = dm.crc16_ccitt((unsigned char*)payload.constData(), bufsize);
		packet.append((unsigned short)(ccrc >> 8) & 0xFF);
		//packet.append((unsigned short)ccrc & 0xFF);
		int retry = 0;
		while (true)
		{
			qDebug() << "send package" << packet;
			port->write(packet);
			if (port->waitForBytesWritten(5000) == false)
				return false;
			//recv
			if (port->waitForReadyRead(5000) == false)
				return false;
			ret = port->readAll();//ack
			qDebug() << "Recv package:" << ret;
			show_message(ret);
			char res = ret[0];

			bool bAck = false;
			switch (res)
			{
			case ACK:
				packetnum++;
				seqnum++;
				bAck = true;
				break;
			case CAN:
				break;
			case NAK:
				break;
			case 'c':
			case 'C':
				continue;
			case '>':
				break;
			default:
				break;
			}

			retry++;
			if (retry>9)
			{
				return false;
			}
			if (bAck)
				break;
		}
		emit progress(floor((float)(packetnum*bufsize) / ba.size()*100.0f));

	}


	QByteArray o_arr;
	o_arr.append(EOT);
	port->write(o_arr);

	if (port->waitForReadyRead(5000) == false)
		return false;
	ret = port->readAll();//ack
	qDebug() << "Recv packet 1 :" << ret;

	if (port->waitForReadyRead(5000) == false)
		return false;
	ret = port->readAll();//ack
	qDebug() << "Recv packet 2 :" << ret;

	if (ret.contains("JOB - OK"))
		return true;

	return false;
}


int FLZCSerialPort::packQR(QByteArray *arr) {
	static OTA_QueryResponse packet = {

		{ 0x40, 0x08, 0x0019, 0x0109, 0x08, 0xFC },					//APS header	
		{ 0x19, 0x81, 0x02 },										//ZCL header
		{ 0x00, 0x1004, 0x0000, 0x30103010, 0x00000000 }			//ZCL Payload

	};

	packet.zclHeader.transSN = packet_number;
	packet.apsHeader.apsCounter = packet_number;
	if (is_ota)
		packet.zclPayload.ImageType = OTA_IT_CONFIG;

	packet.zclPayload.imageSize = bytetoRead;

	arr->append((char*)&packet, sizeof(packet));
	return sizeof(packet);
}

int FLZCSerialPort::packIN(QByteArray *arr) {
	static OTA_ImageNotify packet = {

		{ 0x40, 0x08, 0x0019, 0x0109, 0x08, 0xFC },		//APS header	
		{ 0x19, 0x81, 0x00 },							//ZCL header
		{ 0x00, 0x64 }									//ZCL Payload

	};

	packet.zclHeader.transSN = packet_number;
	packet.apsHeader.apsCounter = packet_number;


	packet.zclPayload.PayloadType = 0;
	packet.zclPayload.QueryJitter = 0x64;

	arr->append((char*)&packet, sizeof(OTA_ImageNotify));
	return sizeof(OTA_ImageNotify);
}

int FLZCSerialPort::packUER(QByteArray *arr) {
	static OTA_UpgradeEndResponse packet = {

		{ 0x40, 0x08, 0x0019, 0x0109, 0x08, 0xFC }, //APS header
		{ 0x19, 0x81, 0x07 },
		{ 0x1004, 0x0000, 0x30103010, 0x00000000, 0x00000000 }

	};

	packet.zclHeader.transSN = packet_number;
	packet.apsHeader.apsCounter = packet_number;
	if (is_ota)
		packet.zclPayload.imgType = OTA_IT_CONFIG;

	packet.zclPayload.curTime = 0;
	packet.zclPayload.ugTime = 0;

	arr->append((char*)&packet, sizeof(packet));
	return sizeof(packet);
}

int FLZCSerialPort::packIBR(QByteArray *arr) {
	static OTA_ImgBlockResponse packet = {
		{ 0x40, 0x08, 0x0019, 0x0109, 0x08, 0xFC },			//APS header	
		{ 0x19, 0x44, 0x05 },								//ZCL header	
		{ 0x00, 0x1004, 0x0000, 0x30103010, 0x00000000,		//ZCL Payload	
		{ 0x30,
		{ 0x00 }
		}
		}

	};

	packet.zclHeader.transSN = data_packet;
	packet.apsHeader.apsCounter = packet_number;

	/*
	packet.zclPayload.fileOffset = inputFileOffset;


	//data_packet++;
	ui.pbarOTA->setValue(inputFileOffset / 48);

	int ms = lapseTimer.elapsed();

	QString out = QString("%1:%2").arg(ms / 60000, 2, 10, QChar('0'))
		.arg((ms % 60000) / 1000, 2, 10, QChar('0'));

	ui.lbTime->setText(QString("Elapsed Time: %1 ").arg(out));
	if (is_ota)
		packet.zclPayload.imgType = OTA_IT_CONFIG;
	else
		packet.zclPayload.imgType = OTA_IT_MANSPC;

	//input.read((char*)packet.zclPayload.imd.Data, 48);
	int imgLen = bytetoRead >= 48 ? 48 : bytetoRead;

	if (inputFileOffset > 0)
	{
		inputFile->seek(inputFileOffset);
		inputFileOffset = 0;
	}
	inputFile->read((char*)packet.zclPayload.imd.Data, imgLen);
	//bytetoRead -= imgLen;

	packet.zclPayload.imd.Length = imgLen;

	arr->append((char*)&packet, sizeof(packet) - 48 + imgLen);
	return (sizeof(packet) - 48 + imgLen);*/
	return 0;
}

void FLZCSerialPort::prepareHeader(QByteArray *arr, bool is_wio,QString WIOAddress)
{
	char header[] = { 0x02, 0x9A, 0x33, 0x00, 0x02, 0x97, 0x8e, 0x09, 0x00, 0x01, 0x01, 0x00, 0x01 };


	bool bOk;

	auto value = WIOAddress;

	if (is_wio)
	{
		value = "0x0000";
	}

	if (value.startsWith("0x", Qt::CaseInsensitive))
		value = value.right(value.length() - 2);

	int i = 5;

	while (value.length() >= 2)
	{
		QString right = value.right(2);
		int hex = right.toInt(&bOk, 16);

		header[i++] = hex;
		value = value.left(value.length() - 2);
	}


	arr->append(header, 13);
}

int FLZCSerialPort::xmodemReceive(QSerialPort* port, QByteArray startChar, int destsz, QString jobfile)
{
	_port = port;
	if (send_command(startChar, _port,false) != 0)
		return Write_OutTime;

	XMODEM* m_xModem = new XMODEM(_port, 0);
	connect(m_xModem, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
	connect(m_xModem, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
	int ret = m_xModem->xmodemReceive(nullptr, 'C', 100, jobfile);
	delete m_xModem;
	m_xModem = 0;
	if (ret == 0 || ret == 1)
		return 0;
	else
		return -1;
}

int FLZCSerialPort::receive(unsigned char *dest)
{	
	return 0;
}

bool FLZCSerialPort::save_data(QDataStream* datasteam, QByteArray& buff1029)
{
	int data_len = 1024;
	int package_len = 1029;
		
	return true;
}

bool FLZCSerialPort::_outbyte(int c, int times)
{

	
	return false;

}