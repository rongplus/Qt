#ifndef FL_ZIGBEE_TEST_H
#define FL_ZIGBEE_TEST_H

#include <QWidget>
#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>

#include "ui_flusbdebug.h"
//#include "flserialread.h"
#include "flxmlcommandmanager.h"
#include "flserialportio.h"
#include <iostream>
#include <fstream>
using namespace std;


class FLFloor;
class FLZoneControllor;
typedef struct Mod
{
	QString org;
	QString mod;
	QString col;
};

typedef struct RouterInfo
{
	int Router;
	QString ShrtAddr;
	QString MacAddr;
	bool valid;
};

class FLZigbeeTest : public QWidget {
	Q_OBJECT
public:
	FLZigbeeTest(QWidget *parent) ;
	~FLZigbeeTest();

	void set_port(QSerialPort* port);
	void set_floor(FLFloor* floor);

	void set_zc(FLZoneControllor * zc);
public slots:
	void refreshCommPorts();
	void onConnect();
	void on_disconnect();
	void onReadyRead();
	void on_toggle_trace();
	void on_get_job();
	void on_send_macs();
	void on_get_firmware();
	void on_command();
	
	void on_send_macs1();

	void on_send_enter();
	
	void on_clear();

	virtual void closeEvent(QCloseEvent *);

	void on_port_changed(int);

	virtual void showEvent(QShowEvent *);
	void commandChanged(const QString &);
	void showPortInfo(int idx);

	void modChanged(int, int);
	void onLog(bool enable);
	void onSaveDir();

	void on_read();
	void on_write();
	void on_login();

	void on_io_state_changed(int);

	void on_textChanged(const QString &);

protected:
	void read();
	void write();

	bool read_cmd(QString cmd, QString filename);

	void write_time();

	void write_daylightsaving();

	void write_passcode();

	void write_firmware();

	void write_configuration();

	void read_time();

	void read_daylightsaving();

	void read_passcode();

	void read_firmware();

	void read_configuration();

	void ProcessMessage(QString mod, QRegularExpressionMatch match);
	QString CmdLookup(QString cmd);
	void LoadMod(void);
	bool eventFilter(QObject *obj, QEvent *event);
	QString ProcessFilter(QString in);

	void InitPort();
	void setSaveCapture(bool enable, QString filename);
	

	Ui::ZigbeeTestUi ui;
	QSerialPort *m_serial;
	//FLSerialRead* m_read_thread;
	bool ready_send_file;

	QList<Mod> mods;
	RouterInfo routers[49];
	FLXMLCommandManager* xml_cmd;
	bool saveCapture;
	QFile * saveFile;
	QString m_dirPath;

	bool auto_login;
	bool _read_oprate;
	FLSerialPortIO* _portIO;
	FLZCSerialPort * _zcport;
	bool read_time_after_login;
	bool write_time_after_login;
};

#endif