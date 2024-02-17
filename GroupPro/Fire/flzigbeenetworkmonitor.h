#ifndef NETWORKVIEW_H
#define NETWORKVIEW_H

#include <QWidget>
#include <QSplitter>
#include <QSerialPortInfo>
#include <QSerialPort>
#include "ui_networkview.h"
#include "flnetview.h"
#include "zigbee.h"
#include "SnifferMain.h"

#include "Switcher.h"
#include "pcap.h"
#include "PCAPReader.h"

class SnifferMC1322x;
class FLFloor;
class FLZoneControllor;

class NetworkView : public QWidget
{
	Q_OBJECT

public:
	NetworkView(QWidget *parent = 0);
	~NetworkView();

	void set_port(QSerialPort * port);
	void openSniffer(QString port,int channel);
	void set_floor(FLFloor* fl);
	void set_zc(FLZoneControllor* zc);
private slots:
	
	void onzbScan(bool enable);
	void onLog(bool enable);
	void onScroll(bool enable);

	void onSaveDir();

	void onPrint();

	void onReadyRead();
	void receivedFrame(const QByteArray& frmBin);
	void ProcessPacket(QByteArray arr, ZBTime zt);
	void readFinished(int packets);
	void openFile();
	void playFile();
	void pauseFile();
	void onOpenJob();
	void floorChanged(int index);

private:

	void showEvent(QShowEvent * event);
	
	int getPanID(QList<QString> list);
	void addItem(QList<QString> list, QString data, int index);
	
	void readPCAP(QString filename);
	void onConnect();
	void openSniffer();
	

	void addTrafficItem(int idx);

	Ui::NetworkView ui;


	
	QSerialPort *m_serial;
	bool is_new_command;
	ZigBeeData zb_data;
	int index;
	int total;

	SnifferMC1322x* sniffer;
	SnifferMain *snifferMain;

	Switcher * onSwitch;
	Switcher * scrollSwitch;

	Switcher * logSwitch;
	bool file;
	int itemIndex;

	QAction *openAction;
	QAction *playAction;
	QAction *pauseAction;
	QAction *stopAction;

	QAction *saveDirAction;

	QLabel *loadFileName;
	PCAPReader *reader;

	QList<flnetview*> nv;
	QList<QString> mPanList;
	QString m_dirPath;
};

#endif // NETWORKVIEW_H