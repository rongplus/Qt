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

#ifndef XMODEM_H
#define XMODEM_H

#include <QObject>
#include <QSerialPort>
#include <QDataStream>

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CTRLZ 0x1A

#define DLY_1S 1000
#define MAXRETRANS 25

class XMODEM : public QObject
{
    Q_OBJECT
public:
	bool m_cancel;
	QSerialPort *m_serialPort;	
	QString m_jobfile;
	int retry_number;
	int _inbyte(unsigned short timeout);	
	void _outbytes(const QByteArray &ba );
	bool _outbyte( int c );
	bool _outbyte( int c ,int times);
	void _flushinput();
	unsigned short crc16_ccitt(const unsigned char *buf, int len);	
	int receive(unsigned char *dest);

	bool save_data(QDataStream* datasteam, QByteArray &arr);
	unsigned char pakager_serials;
	int total_package;
public:
	enum XModemResponce {
		XMODEM_SUCCESS = 0,
		XMODEM_PACKET_FAILED = -1,
		XMODEM_NO_SYNC = -2,
		XMODEM_REMOTE_CANCEL = -3,		
		XMODEM_EOT_ERROR = -4,
		XMODEM_LOCAL_CANCEL = -5,
		XMODEM_RESTART
	};
    explicit XMODEM(QSerialPort *sp, QObject *parent = 0);	
	int xmodemReceive(unsigned char *dest, uchar startChar,int destsz,QString jobfile);
	//int xmodemTransmitOld(unsigned char *src, int srcsz);
	int xTransmit(QByteArray ba, uchar startChar);	
	bool isConnected();
	void cancel();

	static void format_data(QByteArray& ba);
signals:
	void progress(int);
	void message(QString);
};

#endif // XMODEM_H
