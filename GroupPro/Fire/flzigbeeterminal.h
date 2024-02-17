#ifndef FLZIGBEEMONITOR_H
#define FLZIGBEEMONITOR_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>
#include <QMessageBox>
#include <QDomDocument>
#include <QTreeWidgetItem>
#include <QFile>
#include <QElapsedTimer>
#include "ui_flzigbeemonitor.h"
#include "flxmlcommandmanager.h"

#include <iostream>
#include <fstream>
using namespace std;

// OTA Cluster Command Frames
#define COMMAND_IMAGE_NOTIFY                          0x00
#define COMMAND_QUERY_NEXT_IMAGE_REQ                  0x01
#define COMMAND_QUERY_NEXT_IMAGE_RSP                  0x02
#define COMMAND_IMAGE_BLOCK_REQ                       0x03
#define COMMAND_IMAGE_PAGE_REQ                        0x04
#define COMMAND_IMAGE_BLOCK_RSP                       0x05
#define COMMAND_UPGRADE_END_REQ                       0x06
#define COMMAND_UPGRADE_END_RSP                       0x07
#define COMMAND_QUERY_SPECIFIC_FILE_REQ               0x08
#define COMMAND_QUERY_SPECIFIC_FILE_RSP               0x09

enum OTA_MSG_TYPES {
	OTA_IMAGE_NOTIFY,
	OTA_QUERY_RESPONSE,
	OTA_IMAGE_RESPONSE,
	OTA_UPGRADE_END,
};
enum IMAGE_TYPES {
	OTA_IT_MANSPC = 0x0000,
	OTA_IT_CONFIG = 0xffc1,
};

#pragma pack(push, 1)

typedef struct APSHeader_t {
	uint8_t frameControl;
	uint8_t dstEndPoint;
	uint16_t clusterID;
	uint16_t smartEP;
	uint8_t srcEndPoint;
	uint8_t apsCounter;
}APSHeader;


typedef struct ZCLHeader_t {
	uint8_t frameControl;
	uint8_t transSN;
	uint8_t cmdID;
}ZCLHeader;


typedef struct imgData_t {
	uint8_t Length;
	uint8_t Data[65];
}imgData;

typedef struct ZCLPayload_t {
	uint8_t status;
	uint16_t manfCode;
	uint16_t imgType;
	uint32_t fileVer;
	uint32_t fileOffset;
	imgData imd;
}ZCLPayload;



typedef struct ZCLPayload_UER_t {
	uint16_t manfCode;
	uint16_t imgType;
	uint32_t fileVer;
	uint32_t curTime;
	uint32_t ugTime;
}ZCLPayload_UER;


typedef struct ZCLPayload_IN_t {
	uint8_t PayloadType;
	uint8_t QueryJitter;	
}ZCLPayload_IN;


typedef struct ZCLPayload_Query_Response_t {
	uint8_t status;
	uint16_t manfCode;
	uint16_t imgType;
	uint32_t fileVer;
	uint32_t imageSize;
}ZCLPayload_Query_Response;

typedef struct ZCLPayload_Query_Request_t {
	uint8_t FC;
	uint16_t manfCode;
	uint16_t imgType;
	uint32_t fileVer;
	uint8_t Hardware_Ver;
	uint8_t Hardware_Rev;
}ZCLPayload_Query_Request;

typedef struct OTA_QueryRequest_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload_Query_Request zclPayload;
}OTA_QueryRequest;

typedef struct OTA_QueryResponse_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload_Query_Response zclPayload;
}OTA_QueryResponse;

typedef struct OTA_ImageNotify_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload_IN zclPayload;
}OTA_ImageNotify;

typedef struct OTA_ImgBlockResponse_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload zclPayload;
}OTA_ImgBlockResponse;


typedef struct OTA_UpgradeEndResponse_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload_UER zclPayload;
}OTA_UpgradeEndResponse;



#pragma pack(pop)

class FLZigbeeMonitor : public QWidget
{
	Q_OBJECT

public:
	FLZigbeeMonitor(QWidget *parent = 0);
	~FLZigbeeMonitor();

	void set_port(QSerialPort* port);
protected slots:
	/*!
	* refresh the connected usb port 
	*/
	void refreshCommPorts();
	void showPortInfo(int idx);
	void onConnect();
	void on_disconnect();
	void onReadyRead();
	void receive();

	void on_xml_changed(const QString &);
	void on_itemClicked(QTreeWidgetItem * item, int column);
	void on_itemDoubleClicked(QTreeWidgetItem * item, int column);

	void on_edit_textchanged(const QString &);
	void on_select_item_changed(int );
	void on_enum_item_changed(int);

	void on_send_command();
	void on_clear_history();

	
	void on_reset_command(QDomElement* element);
	void on_mostusedtree_Menu(const QPoint & pt);
	
	void on_mostusedtree_DoubleClicked(QTreeWidgetItem * item, int column);

	void find_textChanged(const QString & str);

	//thread message
	void on_comm_msg(int, QStringList,QString);

	void on_data_msg(QStringList, QString);

	void on_expand();

	void onTimer();
	void onOTA_Timer();

	void onOpenJob();

	void onOpenFW();
	void onWriteFW();
	void onCancelFW();

	void onStartDevice();
	void onStopDevice();

private:
	void set_tree_state(QTreeWidget*tree,QPushButton*bt, bool bisible);
	void parseParam(QDomNode command,QWidget* parent_w = nullptr);

	void parse_struct_param(QDomNode command,QComboBox* box);
	void parse_cmd_content(QTreeWidgetItem* item,QDomNode command,QStringList& cmd_list);;

	void set_item(QTreeWidgetItem* item,QStringList &lt);

	void add_child_item(QTreeWidgetItem* item, QStringList &lt);

	QString comm_string(QDomElement* element);

	void parse_rec_data(QByteArray arr);
	void data2frame(QStringList lt);

	void InitTestCommand();

	void excute_command(QDomNode node);
	void excute_command(QString cmd);

	void excute_script_command(QDomNode node);	

	void init_tree(QTreeWidget* tree);
	void clear_tree();
	
	void ProcessData(QByteArray arr);
	
	
	void sendPacket(int type);
	void nextPacket();
	void prepareHeader(QByteArray *arr);

	int packIN(QByteArray *arr);
	int packQR(QByteArray *arr);
	int packIBR(QByteArray *arr);
	int packUER(QByteArray *arr);
	bool ExtractPacket();

	Ui::FLZigbeeMonitor ui;

	QSerialPort *m_serial;
	bool is_new_command;
	int remain_data_size;

	bool ready_send_file;
	QTreeWidgetItem * clicked_item;
	QDomNode cur_param_node;
	QList<QDomNode> rec_comm_list;
	QByteArray data_received;
	
	FLXMLCommandManager* ZigBeePro_cmd;
	FLXMLCommandManager* xml_cmd;

	QTimer _timer;
	QTimer _OTAtimer;


	QString WIOAddress;

	QString binFileName;

	fstream input;
	QFile *inputFile;
	int bytetoRead;

	unsigned char packet_number;
	unsigned char data_packet;
	bool txContinue;
	int inputFileOffset;
	QElapsedTimer lapseTimer;
	bool OTA_active;
	int lastPacket;

	QByteArray rcvBytes;
	int imsize;
};

#endif // FLZIGBEEMONITOR_H
