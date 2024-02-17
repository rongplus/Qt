#include "flserialportio.h"
#include "qx_xmodem.h"
#include <QDebug>

#include "flzcserialport.h"
#pragma region Zone_Controller
FLSerialPortIO::FLSerialPortIO(QObject *parent)
	: QThread(parent)
{
	_zcport = new FLZCSerialPort(this);
	b_running = true;
	b_working = false;
}

FLSerialPortIO::~FLSerialPortIO()
{

}

void FLSerialPortIO::on_port_data()
{
	
}

int FLSerialPortIO::send_data(QString cmd, QStringList res, QByteArray & data, QSerialPort * port)
{	
	mutex.lock();
	_port = port;
	_response = res;
	_data = data;
	_command = cmd;	
	mutex.unlock();
	return 0;
}

void FLSerialPortIO::run()
{	
	while (b_running)
	{
		mutex.lock();
		if (_command == "login")
		{
			b_working = true;
			_command = "";
			run_login();
			b_working = false;
			
		}
		else if(_command.startsWith("get") )
		{
			b_working = true;
			run_read_data();
			_command = "";
			
			b_working = false;
		}
		else if( _command.startsWith("send"))
		{
			b_working = true;
			run_send_data();
			_command = "";
			
			b_working = false;
		}
		else
		{
			sleep(2);
		}
		mutex.unlock();

		

	}
	
}

void FLSerialPortIO::show_modem_message(QString str)
{
	qDebug() << str;
}

void FLSerialPortIO::run_login()
{
	QSerialPort* port = _port;
	Q_ASSERT(port != nullptr);
	static int state = 0;
	QByteArray _out_string = "Password";

	QByteArray R;
	R.append('\r');
	_port->write(R);
	_port->waitForReadyRead(1000);
	QByteArray tmp = _port->readAll();
	qDebug() << "first read  " << tmp;

	QByteArray o_arr;
	//o_arr.append('\r');
	o_arr.append("login");
	o_arr.append('\r');
	qDebug() << "send login == " << o_arr;
	port->write(o_arr);

	

	for (int n = 0; n < 20; n++)
	{
		if (b_running == false)
		{
			emit state_change(-1);
			return;
		}
			
		emit state_change(n);
		
		if (port->isReadable() && port->waitForReadyRead(4000) == false)
		{			

			QByteArray arr_can;
			arr_can.append(CAN);
			arr_can.append('\r');
			port->write(arr_can);

			qDebug() << "send login == " << o_arr;
			port->write(o_arr);
			state = 0;
			continue;
		}
			

		QByteArray arr = port->readAll();
		qDebug() << "read data == " << arr;
		if (_out_string == "Password")
		{
			if (arr.contains(_out_string + ":") == true)
			{

				QByteArray o_arr;
				o_arr.append(_data);
				o_arr.append('\r');
				port->write(o_arr);
				_out_string = "Success";
				qDebug() << "send passowrd == " << o_arr;
				n = 0;
			}
			else if (arr.contains("Root screen") && state == 0)
			{
				state = 1;
			}
			else if (arr.contains("ERROR") && state == 1)
			{
				QByteArray o_arr("\rlogin");
				o_arr.append('\r');
				qDebug() << "send login == " << o_arr;
				port->write(o_arr);
				state = 0;
			}
			else if(arr.contains("C"))
			{
				QByteArray o_arr;
				o_arr.append(CAN);
				o_arr.append('\r');
				port->write(o_arr);
			}

		}
		else if (_out_string == "Success" && arr.contains(_out_string) == true)
		{
			qDebug() << "emit login == " << "Success";
			emit state_change(Login_Success);
			return;
		}
	}

	emit state_change(Failed);
	return;

}

void FLSerialPortIO::run_read_data()
{
	QByteArray arr;
	arr.append(_command);


	if (send_command(arr, _port,false) != 0)
	{	
		emit state_change(Write_OutTime);
		return;
	}

	XMODEM* m_xModem = new XMODEM(_port, 0);
	connect(m_xModem, SIGNAL(progress(int)), this, SIGNAL(progress(int)));
	connect(m_xModem, SIGNAL(message(QString)), this, SLOT(show_modem_message(QString)));
	int ret = m_xModem->xmodemReceive(nullptr, 'C', 100, _filename);
	delete m_xModem;
	m_xModem = 0;
	
	if (ret == 0)
	{
		if(_command == "get time")
			emit state_change(Read_Time_Success);
		else
			emit state_change(DST_Success);
	}
	else
		emit state_change(-1);
	return;
}

void FLSerialPortIO::run_send_data()
{

	if (_zcport->send_binary_1K(_command, _data, _port) != FLZCSerialPort::Write_OutTime)
		//if(send_binary_1K(_command,_data) ==0)
		emit state_change(100);
	else
		emit state_change(-1);
	return;
}


int FLSerialPortIO::send_binary_1K(QString cmd, QByteArray & data)
{
	if (send_command(cmd, _port,false) != 0)
		return Write_OutTime;

	XMODEM md(_port, 0);
	md.format_data(data);
	QByteArray  start;
	start.append('j');
	//md.xTransmit(data, start);

	if (send_data_1K(_port, data) == false)
		return Write_OutTime;

	return Write_Time_Success;

}

int FLSerialPortIO::send_command(QString cmd, QSerialPort * port, bool bWait_c)
{
	Q_ASSERT(port != nullptr);

	QByteArray out_arr(cmd.toStdString().data(), cmd.length());
	out_arr.append('\r');
	port->write(out_arr);

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


bool FLSerialPortIO::send_data_1K(QSerialPort * port, QByteArray & ba)
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
			//emit progress(100);
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
			//show_message(ret);
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
		//emit progress(floor((float)(packetnum*bufsize) / ba.size()*100.0f));

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

#pragma endregion
