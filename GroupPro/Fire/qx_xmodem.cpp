/***************************************************************************************
*                                                                                      *
*	   **************************************************************************      *
*	   **            Company Confidential - For Internal Use Only              **      *
*	   **                   Mircom Group of Companies("MGC")     	           **      *
*	   **                                                                      **      *
*	   **   This information is confidential  and the exclusive property of    **      *
*	   ** Mircom.  It is intended for internal use and only for the purposes   **      *
*	   **   provided,  and may not be disclosed to any third party without     **      *
*	   **                prior written permission from Mircom.                 **      *
*	   **                                                                      **      *
*	   **                          Copyright 2015                              **      *
*	   **                            Omid Sakhi                                **      *
*	   **************************************************************************      *
*																					   *
***************************************************************************************/

#ifndef STR
#define STR(x) x.toUtf8().data()
#endif

#include "qx_xmodem.h"
#include <glog/logging.h>
#include <QSerialPort>
#include <QFile>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>

XMODEM::XMODEM( QSerialPort *sp, QObject *parent /*= 0*/ ) : QObject(parent)
{
	m_serialPort = sp;
	m_cancel = false;
	m_jobfile = "job.bin";
}

static const unsigned short crc16tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

unsigned short XMODEM::crc16_ccitt( const unsigned char *buf, int len )
{
	register int counter;
	register unsigned short crc = 0;
	for( counter = 0; counter < len; counter++)
		crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf++)&0x00FF];
	return crc;
}

int XMODEM::_inbyte( unsigned short timeout )
{
	m_serialPort->waitForReadyRead(timeout);
	return m_serialPort->read(1)[0];	
}

void XMODEM::_outbytes(const QByteArray &ba )
{	
	m_serialPort->write(ba);	
	m_serialPort->waitForBytesWritten(1000);
}

bool XMODEM::_outbyte( int c )
{
	QByteArray ba;
	ba.append(QChar(c));
	m_serialPort->write(ba);
	return m_serialPort->waitForBytesWritten(1000);
}

bool XMODEM::_outbyte( int c, int times )
{
	
	QByteArray ba;
	ba.append(QChar(c));
	

	for(int retry = 0; retry < times && !m_cancel; ++retry)
	{
		LOG(INFO) <<   "Write USB serials data = " + ba.toStdString();
		m_serialPort->write(ba);
		if( m_serialPort->waitForBytesWritten(1000) == true)
			return true;
		LOG(INFO) <<   "Error Write USB serials data = " + ba.toStdString();
	}
	
	return false;
	
}
void XMODEM::format_data(QByteArray& ba)
{
	unsigned int bufsize = 1024;
	//LOG(INFO) << "XMODEM::xTransmit - size " << ba.size();
	if (ba.size() % bufsize != 0)
	{
		// Space padding in the buffer		
		const char ctrlz[2] = { CTRLZ,0x00 };
		QString pad(ctrlz);
		pad = pad.repeated((bufsize - (ba.size() % bufsize)));
		ba.append(pad.toUtf8());
		qDebug() << "XMODEM::xTransmit - size with padding" << ba.size();
	}
}

void XMODEM::_flushinput()
{
	m_serialPort->readAll();
}

int XMODEM::xTransmit( QByteArray ba , uchar startChar)
{
	unsigned int bufsize = 1024;		
	LOG(INFO) << "XMODEM::xTransmit - size " << ba.size();
	if (ba.size()%bufsize != 0)
	{
		// Space padding in the buffer		
		const char ctrlz[2] = {CTRLZ,0x00};
		QString pad(ctrlz);
		pad = pad.repeated((bufsize-(ba.size()%bufsize)));
		ba.append(pad.toUtf8());		
		LOG(INFO) << "XMODEM::xTransmit - size with padding" << ba.size();
	}	
	
	int ret;
	for(int retry = 0; retry < 50 && !m_cancel; ++retry)
	{
		_outbyte(startChar); // start request
		_flushinput();
		int c;
		if ((c = _inbyte((DLY_1S)<<1)) >= 0) {
			switch (c) {
				case 'c':
				case 'C':
					LOG(INFO) << "C";
				case NAK:
					{					
					LOG(INFO) << "NAK";
					// Begin Transmission
					quint32 packetnum = 0;
					quint8 seqnum = 1;
					quint32 ret = 0;
					quint32 res = 0;
					
					QByteArray payload, packet;

					while (true)
					{						
						if (m_cancel)
						{							
							_outbyte(CAN);
							_outbyte(CAN);
							_outbyte(CAN);
							_flushinput();
							ret = XMODEM_LOCAL_CANCEL;
							break;
						}

						if (packetnum*bufsize+1 >= ba.size())
						{							
							emit progress(100);
							LOG(INFO) << "XMODEM::xBeginTransmit - EOT";		
							for (int retry = 0; retry < 10; ++retry) {
								_outbyte(EOT);
								if ((res = _inbyte((DLY_1S)<<1)) == ACK) break;
							}
							ret = (res == ACK)?packetnum*(bufsize+1):XMODEM_EOT_ERROR;
							break;
						}	

						payload = ba.mid(packetnum*bufsize,bufsize);
						packet.clear();
						packet.append(QString(STX).toUtf8());
						packet.append(seqnum);
						packet.append(~seqnum);
						packet.append(payload);
						unsigned short ccrc = crc16_ccitt((unsigned char*)payload.constData(),bufsize);
						packet.append((unsigned short)(ccrc>>8) & 0xFF);
						packet.append((unsigned short)ccrc & 0xFF);
						
						//LOG(INFO) << "Packet # : " << packetnum << "Packet Size : " << packet.size() << "Total Packets : " << (float)ba.size() / bufsize;

						emit message(QString("Sending packet %1/%2").arg(packetnum).arg((float)ba.size()/bufsize));
						emit progress(floor((float)(packetnum*bufsize)/ba.size()*100.0f));
						LOG_IF(FATAL,packet.size() != 1029) << "PACKET_SIZE != 1029";
						int retry = 0;
						bool ack = false;
						while(!m_cancel && !ack)
						{
							_outbytes(packet);
							if ((res = _inbyte(DLY_1S)) >= 0 ) {
								switch (res) {
								case ACK:										
									ack = true;
									break;
								case CAN:									
									if ((res = _inbyte(DLY_1S)) == CAN) {
										_outbyte(ACK);
										_flushinput();
										//LOG(INFO) << "XMODEM::xBeginTransmit - canceled by remote";					
										ret = XMODEM_REMOTE_CANCEL;
										break;	
									}
									break;
								case NAK:																		
									break;
								case 'c':
								case 'C':									
									retry--;
									continue;
								case '>':
									ret = XMODEM_RESTART;
									break;
								default:
									LOG(INFO) << "UNKNOWN " << res;				
									break;
								}
							}
							retry++;
							if (retry>9)
							{
								_outbyte(CAN);
								_outbyte(CAN);
								_outbyte(CAN);
								_flushinput();
								if (m_cancel)
									ret = XMODEM_LOCAL_CANCEL;
								else
									ret = XMODEM_PACKET_FAILED;
								break;
							}
						}						
						packetnum++;						
						seqnum++;
					}					
					if (ret == XMODEM_RESTART)
					{
						retry = 0;
						continue;
					}
					return ret;
					}
					break;
				case CAN:
					if ((c = _inbyte(DLY_1S)) == CAN) {
						_outbyte(ACK);
						_flushinput();						
						LOG(INFO) << "XMODEM::xTransmit - canceled by remote";
						return XMODEM_REMOTE_CANCEL;
					}
					break;
				default:		
					LOG(INFO) << "UNKNOWN : " << c;
					break;
			}
		}
	}
	_outbyte(CAN);
	_outbyte(CAN);
	_outbyte(CAN);
	_flushinput();
	LOG(INFO) << "NO SYNC";
	if (m_cancel)
		return XMODEM_LOCAL_CANCEL;
	else
		return XMODEM_NO_SYNC;
}

bool XMODEM::isConnected()
{
	return m_serialPort->isOpen();
}

void XMODEM::cancel()
{
	m_cancel = true;
}

int XMODEM::xmodemReceive(unsigned char *dest,uchar startChar, int destsz,QString jobfile)
{
	unsigned int bufsize = 1024;	
	retry_number =25;
	m_jobfile = jobfile;
	int ret;
	for(int retry = 0; retry < 50 && !m_cancel; ++retry)
	{
		//if( _outbyte(startChar,50) == false)
		//	return -1;
		//if( m_serialPort->waitForReadyRead(1000) == false)
		//	continue;
		//QByteArray rev_arr = m_serialPort->readAll();
		
		//if( rev_arr[0] == 'G')
		{
			emit message( "start receive job");
			int ret =  receive(dest);	
			emit message(  "end receive job");
			
			if(ret == 0)
				_outbyte(ACK, 50);
			else
				_outbyte(CAN, 50);

			//check file
			
			return ret;
		}
	}

	return XMODEM_NO_SYNC;
}
int XMODEM::receive(unsigned char *dest)
{

	QFile job(m_jobfile);
	if(job.open(QIODevice::WriteOnly) == false)
		emit message( "Open file error xmodem");
	QDataStream* datasteam = new QDataStream(&job);

	if(_outbyte('C',20)== false)
	{
		return -1 ;
	}
	emit message( "waiting data from panel");
	int package_len = 1029;
	QByteArray buff1024;
	pakager_serials = 1;
	total_package = 1;
	int waittimes = 20;
	int c_times = 0;
	bool start_tranfer = true;
	while (true)
	{
		if(start_tranfer == false)
		{
			c_times++;
			if(c_times<16)
			{
				if(_outbyte('C',20)== false)
				{
					return -1;
				}
			}
			else
				return -1;
		}

		if(m_serialPort->waitForReadyRead(1000) == false)
		{
			waittimes--;
			if(waittimes<0)
				return -2;
			continue;
		}
		waittimes = 20;
		
		QByteArray arr = m_serialPort->readAll();
		//qDebug() << "data length = " +  QString::number(arr.length()) + "data = " + arr ;


		for(int n=0; n< arr.length();n++)
		{
			int c = arr[n];
			if( c == EOT )
			{
				qDebug() << "Get EOT " << arr.length() ;
				save_data(datasteam,buff1024);
				datasteam->device()->close();
				return 1;
			}
			else if( c == CAN)
			{
				qDebug() << "Get CAN " << arr.length() ;
				break;
			}
			else if(c == STX)
			{
				start_tranfer = true;	
				buff1024.append(arr.left(n));
				//qDebug() << arr;
				if( save_data(datasteam,buff1024) == false)
				{
					datasteam->device()->close();
					return -3;
				}
				buff1024.clear();

				buff1024.append( arr.mid(n,arr.length()));
				int waittimes = 20;

				while( buff1024.size() < package_len)
				{
					if(m_serialPort->waitForReadyRead(1000) == false)
					{
						waittimes--;
						if(waittimes<0)
							return -1;
						continue;
					}
					waittimes = 20;

					QByteArray buff = m_serialPort->readAll();
					buff1024.append( buff);
				}
				if( save_data(datasteam,buff1024) == false)
				{
					datasteam->device()->close();
					return -3;
				}
// 				if(_outbyte(NAK,20)== false)
// 				{
// 					return -1;
// 				}
				break;
				//start_recv(arr,n);
							
			}
			else if(c < 0)
			{
				buff1024.clear();
				if(_outbyte(ACK,20)== false)
				{
					return -1;
				}
				break;
			}
			
		}
		
	}

	save_data(datasteam,buff1024);
	datasteam->device()->close();
	return 0;
}
bool XMODEM::save_data(QDataStream* datasteam, QByteArray& buff1029)
{
	int data_len = 1024;
	int package_len = 1029;

	while(buff1029.length() >= package_len)
	{
		unsigned char crc = buff1029[data_len+3];
		char * ppp = (char *)buff1029.data();
		QByteArray arr = buff1029.left(data_len+3);
		//qDebug() << "-->data = " << arr;
		
		if( (unsigned int)arr[0] != STX || pakager_serials != (unsigned int)arr[1]  || ((unsigned char)arr[1] != (unsigned char)(~arr[2])))
		{
			//emit message (QString("Get Wrong data: head = %1 package number = %2 package number=%3" ).arg((unsigned int)arr[0]).arg((unsigned int)arr[1] ).arg( pakager_serials));
			//qDebug() << "Wrong data: head = " <<(unsigned int)arr[0] << "package number " <<(unsigned int)arr[1] << "!= " << pakager_serials;			
			//buff1024.clear();
			_outbyte(NAK,50);	
		}
		else
		{			
			arr = arr.right(data_len);				
			unsigned short data_crc = crc16_ccitt((unsigned char*)arr.constData(),arr.length());
 			int crc2 = (unsigned short)(data_crc>>8) & 0xFF;
			
			if(crc2 != crc)
			{
				//buff1024.clear();
				//_outbyte(NAK,50);	
				qDebug() << "crc  data_crc = " << crc << crc2 ;
			}
			else
			{
				retry_number = 25;
				datasteam->writeRawData(arr.constData(),arr.length());
				buff1029 = buff1029.right( buff1029.length() - package_len);

				emit message (QString("Get data: package %1" ).arg( total_package));
				emit progress(total_package %100);		

				pakager_serials++;
				total_package++;

				_outbyte(ACK,50);	

			}
		}

		retry_number--;
		if(retry_number < 0)
			return false;
	
		
	}
	return true;
}
