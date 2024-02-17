#include "qx_link.h"

#include <QDebug>
#include <QString>
#include <QStringList>
#include <glog/logging.h>

QxLink::QxLink( QObject *parent /*= 0*/ ) : QThread(parent)
{
	//LOG(INFO) << "Hello QxLink";		
	m_exit = false;
	m_notify = false;
	m_serialPort = 0;
	m_xModem = 0;
}

QxLink::~QxLink()
{
	LOG(INFO) << "Bye QxLink";
}

int QxLink::chat( QByteArray snd, QStringList rcv, int msec, QString *response )
{
	if (!snd.isEmpty())
	{
		for (int i=0;i<snd.count();i++)
		{
			char temp = snd.at(i);
			m_serialPort->write(&temp,1);
			msleep(50);
		}
		m_serialPort->waitForBytesWritten(msec/2);
	}
	msleep(50);
	bool readyRead;
	QByteArray res;
	int retry = 30;
	while(readyRead = m_serialPort->waitForReadyRead(msec) && retry>0)
	{		
		res += m_serialPort->readAll();		
		for (int i=0;i<rcv.count();++i)
			if (res.contains(rcv[i].toUtf8().data()))
			{
				if (response)
					*response = res;
				return i+1;
			}
			retry--;
			msleep(50);
	}
	return readyRead?-1:0;
}
bool QxLink::Check()
{
	bool success = false;
	for (int i=0;i<20;++i) // retry
	{
		emit progress(i*5);
		QString response;
		chat(" ",QStringList() << "Success" << "Fail",2000,&response);			
		if (response.contains("Success"))
		{
			emit message("Check Success");
			success = true;	
			return true;
		}
	}	
	
	 return false;
}

void QxLink::run()
{
	//LOG(INFO) << "Starting QxLink";		
	
	m_firmware_version.clear();
	m_job_version.clear();
	m_job_ID.clear();
	
	m_serialPort = new QSerialPort(m_port,0);
	if (!m_serialPort->open(QIODevice::ReadWrite))
	{
		//LOG(WARNING) << "Oops! Could not open serial port " << STR(m_port);
		emit error("Opening serial port [ failed ]");
		delete m_serialPort;
		m_serialPort = 0;
		return;
	}
	bool check = true;		
	check = check &	m_serialPort->setBaudRate(QSerialPort::Baud115200);
	check = check & m_serialPort->setDataBits(QSerialPort::Data8);
	check = check & m_serialPort->setStopBits(QSerialPort::OneStop);
	check = check & m_serialPort->setParity(QSerialPort::NoParity);
	check = check & m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
	if (check == false)
	{
		//LOG(WARNING) << "Oops! Could not set parameters on " << STR(m_port);
		emit error("Setting serial port parameters [ failed ].");
		m_serialPort->close();
		delete m_serialPort;
		m_serialPort = 0;
		return;
	}	
	for(int retry = 0; retry< 20; ++retry)
	{
		QString str_hard;
		chat("\r",QStringList() << "Ver" << "version",50,&m_firmware_version);
		QRegExp reg("\\d+(\\.\\d+)+");
		int pos = reg.indexIn(m_firmware_version);
		str_hard = m_firmware_version;
		if (pos > -1)
		{
			m_firmware_version = reg.cap(0);

			pos = str_hard.indexOf("Job ID");
			QString guid = "D7181948-F0D0-4421-8790-7A6C9EDA1FB9";	
			if(pos > -1)
			{
				str_hard = str_hard.right(str_hard.size() - pos-7);
				m_job_ID = str_hard.left(guid.size()+1);
				str_hard = str_hard.right(str_hard.size() - guid.size());
			}
			QRegExp reg1("\\d+");
			pos = reg.indexIn(str_hard);
			if(pos > -1)
			{
				m_job_version = reg.cap(0);
			}

			break;
		}
		
	}
	if (m_firmware_version.isEmpty())
	{
		//LOG(WARNING) << "Oops! Could not read firmware version on " << STR(m_port);
		emit error("Failed to get the firmware version.\nEither you have selected the wrong port or the panel is really busy.");
		m_serialPort->close();
		delete m_serialPort;
		m_serialPort = 0;
		return;
	}	
	m_exit = false;
	
	emit connected();
		
	while (!m_exit)
	{
		if (isConnected() && !isBusy())
		{
			if(m_job_version.isEmpty())
			{
				for(int retry = 0; retry< 20; ++retry)
				{
					chat1("\r",QStringList() << "version" <<"version" ,50,&m_job_version);
					QRegExp reg("\\d+");
					int pos = reg.indexIn(m_job_version);
					if (pos > -1)
					{
						m_job_version = reg.cap(0);	
						emit updatePanel();
						break;
					}
				}
			}

			m_mux.lock();
			QueueItem *item = NULL;
			if (!m_normalQueue.isEmpty())
			{
				item = m_normalQueue.front();
				m_normalQueue.pop_front();
			}
			else if (!m_idleQueue.isEmpty())
			{
				item = m_idleQueue.front();
				m_idleQueue.pop_front();
			}
			m_currentItem = item;
			m_mux.unlock();
			if (item != NULL)
			{				
				m_notify = true;					
				if (SendDataItem *q = qobject_cast<SendDataItem*>(item))
				{
					m_xModem = new XMODEM(m_serialPort,0);
					connect(m_xModem,SIGNAL(progress(int)),this,SIGNAL(progress(int)));
					connect(m_xModem,SIGNAL(message(QString)),this,SIGNAL(message(QString)));
					emit transmissionStarted();
					int ret = 0;// m_xModem->xTransmit(q->ba, q->startChar);
					disconnect(m_xModem,SIGNAL(progress(int)),this,SIGNAL(progress(int)));
					disconnect(m_xModem,SIGNAL(message(QString)),this,SIGNAL(message(QString)));
					if (ret < 0)
					{
						emit transmissionError();
						emit message(QString("XModem Transmission [ failed %1 ]").arg(ret));
					}
					else
					{
						bool bSuc = true;
						if(q->startChar == 'P' && firmwareVersion().major == 2)
						{
							emit message("Checking firmware integrity ..");
							emit progress(0);
							bSuc = Check();
						}
						if(bSuc)
						{
							emit transmissionFinished();
							emit message("Checking firmware was successful!");
						}
						else
						{
							emit transmissionError();
							emit message("Checking firmware was faild!");
						}
					}
					delete m_xModem;
					m_xModem = 0;
				}
				m_currentItem = 0;
				delete item;
				item = nullptr;
			}
			else
			{
				if (m_notify)
				{
					emit fetchedAll();
					m_notify = false;
				}					
			}
		} // if (isConnected() && !isBusy())
		else
		{
			if (m_exit == false && !isConnected())
			{
				//LOG(WARNING) << "Oops! Panel disconnected unexpectedly!";
				emit error("Panel disconnected unexpectedly!");
				break;
			}
		}
		msleep(300);
	}
	m_normalQueue.clear();
	m_idleQueue.clear();		
	m_serialPort->deleteLater();
	m_serialPort = 0;
	emit disconnected();
	//LOG(INFO) << "QxLink disconnected.";
}

void QxLink::send( QByteArray ba,QueueItemPriority priority, uchar startChar )
{
	SendDataItem *item = new SendDataItem;
	item->ba = ba;
	item->startChar = startChar;
	m_mux.lock();
	if (priority == ImmediatePriority)
		m_normalQueue.push_front(item);
	else if (priority == NormalPriority)
		m_normalQueue.push_back(item);
	else if (priority == IdlePriority)
		m_idleQueue.push_back(item);		
	m_mux.unlock();
}

bool QxLink::isConnected()
{
	return m_serialPort && m_serialPort->isOpen();
}

bool QxLink::isBusy()
{
	return m_xModem != NULL;
}

FirmwareVersion QxLink::firmwareVersion()
{
	FirmwareVersion ver;
	ver.major = 0;
	ver.minor = 0;
	ver.patch = 0;
	QStringList nums = m_firmware_version.split(".");
	if (nums.size()>0)
		ver.major = nums[0].toInt();
	if (nums.size()>1)
		ver.minor = nums[1].toInt();
	if (nums.size()>2)
		ver.patch = nums[2].toInt();
	return ver;
}

void QxLink::cancel()
{
	//LOG(INFO) << "User canceled XMODEM.";
	if (m_xModem != NULL)
	{
		m_xModem->cancel();
	}
}

void QxLink::exitLoop()
{
	m_exit = true;
}

QString QxLink::firmwareVersionString()
{
	return m_firmware_version;
}

int QxLink::compareVersion( int major, int minor, int patch )
{
	FirmwareVersion ver = firmwareVersion();
	if (ver.major == major && ver.minor == minor && ver.patch == patch)
		return 0;
	if (major < ver.major)
		return -1;
	else
	{
		if (minor < ver.minor)
			return -1;
		else
		{
			if (patch < ver.patch)
				return -1;
		}
	}
	return 1;
}


int QxLink::chat1( QByteArray snd, QStringList rcv, int msec, QString *response )
{
	if (!snd.isEmpty())
	{
		for (int i=0;i<snd.count();i++)
		{
			char temp = snd.at(i);
			m_serialPort->write(&temp,1);
			msleep(50);
		}
		m_serialPort->waitForBytesWritten(msec/2);
	}
	msleep(50);
	bool readyRead;
	QByteArray res;
	int retry = 30;
	while(readyRead = m_serialPort->waitForReadyRead(msec) && retry>0)
	{		
		res = m_serialPort->readLine();		
		for (int i=0;i<rcv.count();++i)
			if (res.contains(rcv[i].toUtf8().data()))
			{
				if (response)
					*response = res;
				return i+1;
			}
			retry--;
			msleep(50);
	}
	return readyRead?-1:0;
}
