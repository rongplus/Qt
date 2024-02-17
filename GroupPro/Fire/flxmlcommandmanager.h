#ifndef FLXMLCOMMANDMANAGER_H
#define FLXMLCOMMANDMANAGER_H

#include <QObject>
#include <QDomDocument>
#include <QMap>
#include <QSerialPort>

class FLXMLCommandManager : public QObject
{
	Q_OBJECT

public:
	FLXMLCommandManager(QString xmf_file,QObject *parent=0);
	FLXMLCommandManager( QObject *parent = 0);
	~FLXMLCommandManager();

	static FLXMLCommandManager* GetZigBeeProCMD();

	static void excute_cmd(QString xml, QString cmd, QSerialPort*port);
	static void excute_most_cmd(QString xml, QString cmd, QSerialPort*port);

	static void excute_cmd(QDomNode node, QSerialPort* port);
	static void excute_script_cmd(QDomNode script_node, QSerialPort * port);

	void loadxml(QString file_name);
	
	QDomNode find_node_byName(QString name);
	QDomNode find_node_byHeader(QString header);

	QMap<QString, QList<QDomNode>> all_cmd_group();
	QList<QDomNode> all_commmand();

	QByteArray FLXMLCommandManager::command_default_array(QDomNode node);		
private:
	void parse_cmd_group(QString group_name, QDomNode node, QMap<QString, QList<QDomNode>> &map);
	void  init_rec_command();

	QString _file_name;
	QMap<QString, QList<QDomNode>> cmd_collection;
	
	QList<QDomNode> res_comm_list;

	static FLXMLCommandManager* ZigBeeProCMD;
};

#endif // FLXMLCOMMANDMANAGER_H
