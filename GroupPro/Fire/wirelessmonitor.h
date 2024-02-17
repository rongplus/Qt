#ifndef WIRELESSMONITOR_H
#define WIRELESSMONITOR_H
#ifndef UNICODE
#define UNICODE
#endif
#ifndef  _WIN32_WINNT
#define  _WIN32_WINNT _WIN32_WINNT_WINXP
#endif

#pragma comment(lib, "version.lib")
// Need to link with Wlanapi.lib and Ole32.lib
//XP x64 SP2 did not have wlanapi.dll in system32? why?
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "Wininet.lib")

#include <iostream>
#include <io.h>
#include <fcntl.h>

//#include "GlobalFunctions.h"
#include <atlstr.h>

#include <windows.h>
#include <wlanapi.h>
#include <Windot11.h>           // for DOT11_SSID struct
#include <objbase.h>
#include <wtypes.h>
#include <string>
#include <atlbase.h>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
//#include <versionhelpers.h>

#include <WinInet.h>

#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif

//#include "guicon.h"
#include <QWidget>
#include <qsplitter.h>

#include "ui_wirelessmonitor.h"
#include "bargraph.h"
#include "ledindicator.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include "zigbee.h"
#include <QMessageBox>
class WirelessMonitor : public QWidget
{
	Q_OBJECT

public:
	WirelessMonitor(QWidget *parent = 0);
	~WirelessMonitor();
	void set_port(QSerialPort * port);

protected slots:
	void startEneryScan();
	void onReadyRead();
	void onScan(bool enable);
	void onzbScan(bool enable);
	void updateGraph();
	void updateZBGraph();
	void resizeEvent(QResizeEvent * event);
private:
	
	void onConnect();
	DWORD doConsoleScan();
	void GetBList(HANDLE hClient, PWLAN_INTERFACE_INFO pIfInfo );
	void insertText(QString text, int i, int j);
	
	std::vector<int> Channels;
	std::vector<int> RSSIs;
	std::vector<QString> SSIDs;

	std::vector<int> zbChannels;
	std::vector<int> zbRSSIs;
	std::vector<QString> zbSSIDs;


	QSerialPort *m_serial;
	bool is_new_command;
	ZigBeeData zb_data;
	int index;
	int total;

	BarGraph *bg;
	BarGraph *zb_bg;

	LedIndicator *wifiLedIndicator;
	LedIndicator *zigbeeLedIndicator;
	QSplitter *sp;

	QTimer *timer;
	QTimer *zbtimer;

	HANDLE hClient;

	Ui::WirelessMonitor ui;
};

#endif // WIRELESSMONITOR_H
