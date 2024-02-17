#include "flxmlcommandmanager.h"
#include <QFile>
#include <QDebug>
#include <QApplication>
#include "global.h"

FLXMLCommandManager* FLXMLCommandManager::ZigBeeProCMD = nullptr;

FLXMLCommandManager::FLXMLCommandManager(QString xml_file, QObject *parent)
	: QObject(parent)
{
	loadxml(xml_file);
	_file_name = xml_file;
}

FLXMLCommandManager::FLXMLCommandManager(QObject *parent)
	: QObject(parent)
{

}

FLXMLCommandManager::~FLXMLCommandManager()
{

}

FLXMLCommandManager * FLXMLCommandManager::GetZigBeeProCMD()
{
	if (ZigBeeProCMD == nullptr)
	{
		ZigBeeProCMD = new FLXMLCommandManager();
		auto file_path = QApplication::applicationDirPath() + "/zigbee/";
		file_path += "ZigBeePro.xml";
		ZigBeeProCMD->loadxml(file_path);

	}
	return ZigBeeProCMD;
}

void FLXMLCommandManager::loadxml(QString xml)
{
	_file_name = xml;
	cmd_collection.clear();
	QFile xml_file(xml);
	QDomDocument dom;
	if (!xml_file.open(QIODevice::ReadOnly))
		return;
	//QString manifest = xml_file.readAll();

	if (!dom.setContent(&xml_file)) {
		xml_file.close();
		return;
	}

	xml_file.close();

	//dom.setContent(manifest);
	if (dom.isNull() || (!dom.isDocument()))
	{
		qDebug() << "QxPluginServer::parseManifest: Failed to parse the manifest file!";
	}

	//QDomNodeList pluginNodes = dom.elementsByTagName("plugin");
	QDomElement docElem = dom.documentElement();//zigbee

	auto node = docElem.firstChild();//commands

	
	while (!node.isNull())
	{
		QDomElement e = node.toElement(); // try to convert the node to an element.
		qDebug() << qPrintable(e.tagName()); // the node really is an element.
		if (e.tagName() == "Commands" )
		{
			//parseCommandGroup(ui.treeWidget, node.firstChild());
			auto cmd_group_node = node.firstChild();
			while (cmd_group_node.isNull() == false)
			{
				parse_cmd_group("", cmd_group_node, cmd_collection);
				cmd_group_node = cmd_group_node.nextSibling();
			}
		}
		else if(e.tagName() == "Events")
		{
			//parseCommandGroup(ui.treeWidget, node.firstChild());
			auto cmd_group_node = node.firstChild();
			while (cmd_group_node.isNull() == false)
			{
				parse_cmd_group("", cmd_group_node, cmd_collection);
				cmd_group_node = cmd_group_node.nextSibling();
			}
		}
		node = node.nextSibling();
	}
		
	init_rec_command();
}

void FLXMLCommandManager::parse_cmd_group(QString group_name, QDomNode node, QMap<QString, QList<QDomNode>> &map)
{
	auto cmd = node.firstChild();
	if (cmd.toElement().tagName() == "GroupDesc")
	{
		group_name = cmd.toElement().text();
	}
	else
	{
		group_name = node.toElement().tagName();
	}
		
	QList<QDomNode> list = map[group_name];

	while (cmd.isNull() == false)
	{
		if (cmd.toElement().tagName() == "Cmd")
		{
			list.append(cmd);
		}
		cmd = cmd.nextSibling();
	}

	map[group_name] = list;
}

QMap<QString, QList<QDomNode>> FLXMLCommandManager::all_cmd_group()
{
	return cmd_collection;
}

QByteArray FLXMLCommandManager::command_default_array(QDomNode node)
{	
	auto comm_text = node.firstChildElement("CmdHeader").text();
	auto head_list = comm_text.split(" ");

	bool bOk;
	QByteArray arr;
	//header
	char TX = 0x02;
	arr.append(TX);
	foreach(auto str, head_list)
	{
		int hex = str.toInt(&bOk, 16);
		arr.append(hex);
	}
	arr.append('0');
	
	//parameter;
	auto params = node.firstChildElement("CmdParms");
	auto param_node = params.firstChild();

	while (param_node.isNull() == false)
	{
		auto param_element = param_node.toElement();

		//default value ;
		QString default_value = param_element.firstChildElement("ParmDefaultValue").text();
		auto param_name = param_element.firstChildElement("ParmName").text();
		if (default_value.isEmpty())
			default_value = "00";
		auto type_node = param_element.firstChildElement("ParmType");

		// 		if(type_node.text() == "tEnum")
		// 		{
		// 		}
		// 		else if(type_node.text() == "tEdit" )
		// 		{			
		// 		}
		// 		else if(type_node.text() == "tInt")
		// 		{
		// 		}
		// 		else if(type_node.text() == "tBool")
		// 		{
		// 		}
		// 		else 
		if (type_node.text() == "tBitArray")
		{

		}
		// 		else if(type_node.text() == "tArray")
		// 		{
		// 			
		// 		}
		else if (type_node.text() == "tSelect")
		{

		}
		auto param_list = default_value.split(" ");
		foreach(auto param, param_list)
		{
			int hex = param.toInt(&bOk, 16);
			arr.append(hex);
		}		
		param_node = param_node.nextSibling();

	}


	char ch_len = arr.length() - 4;
	arr[3] = ch_len;

	arr.append(Checksum(arr, arr.length()));

	return arr;
}

void  FLXMLCommandManager::init_rec_command()
{
	res_comm_list.clear();
	QMap<QString, QList<QDomNode>>::const_iterator it = cmd_collection.constBegin();
	while (it != cmd_collection.constEnd())
	{
		auto list = it.value();
		foreach(auto node, list)
		{
			auto comm_node = node.toElement();
			if (comm_node.tagName() == "Cmd")
			{
				//auto comm_text = comm_node.firstChildElement("CmdName").text();
				//if (comm_text.endsWith("Response", Qt::CaseInsensitive) || comm_text.endsWith("Confirm", Qt::CaseInsensitive)
				//	|| comm_text.endsWith("event", Qt::CaseInsensitive) || comm_text.endsWith("Indication", Qt::CaseInsensitive))
				{
					//add to response list;
					res_comm_list.append(comm_node);
				}
			}
		}
		it++;
	}
}

QList<QDomNode> FLXMLCommandManager::all_commmand()
{
	if (res_comm_list.count() == 0)
		init_rec_command();
	return res_comm_list;
}

QDomNode FLXMLCommandManager::find_node_byHeader(QString strHeader)
{
	foreach(auto node, res_comm_list)
	{
		auto head = node.firstChildElement("CmdHeader");
		if (head.text().remove(" ").compare(strHeader,Qt::CaseInsensitive) == 0 )
		{
			return node;
		}
	}
	return QDomNode();
}

QDomNode FLXMLCommandManager::find_node_byName(QString name)
{
	QMap<QString, QList<QDomNode>>::const_iterator it = cmd_collection.constBegin();
	while (it != cmd_collection.constEnd())
	{
		auto list = it.value();
		foreach(auto node, list)
		{
			auto node_name = node.firstChildElement("CmdName").text();
			if (node_name == name)
			{
				return node;
			}

		}
		it++;
	}
	return QDomNode();
}

void FLXMLCommandManager::excute_cmd(QString xml, QString cmd, QSerialPort*port)
{
	FLXMLCommandManager* mg = new FLXMLCommandManager(0);
	Q_ASSERT(QFile::exists(xml));
	mg->loadxml(xml);
	auto node = mg->find_node_byName(cmd);
	Q_ASSERT(node.isNull() == false);
	delete mg;


	auto comm_text = node.firstChildElement("CmdHeader").text();
	auto head_list = comm_text.split(" ");

	bool bOk;
	QByteArray arr;
	//header
	char TX = 0x02;
	arr.append(TX);
	foreach(auto str, head_list)
	{
		int hex = str.toInt(&bOk, 16);
		arr.append(hex);
	}
	arr.append('0');

	//parameter;
	auto params = node.firstChildElement("CmdParms");
	auto param_node = params.firstChild();

	while (param_node.isNull() == false)
	{
		auto param_element = param_node.toElement();

		//default value ;
		QString default_value = param_element.firstChildElement("ParmDefaultValue").text();
		auto param_name = param_element.firstChildElement("ParmName").text();
		if (default_value.isEmpty())
			default_value = "00";
		auto type_node = param_element.firstChildElement("ParmType");


		if (type_node.text() == "tBitArray")
		{

		}

		else if (type_node.text() == "tSelect")
		{

		}
		auto param_list = default_value.split(" ");
		foreach(auto param, param_list)
		{
			int hex = param.toInt(&bOk, 16);
			arr.append(hex);
		}
		param_node = param_node.nextSibling();

	}


	char ch_len = arr.length() - 4;
	arr[3] = ch_len;

	arr.append(Checksum(arr, arr.length()));

	QString str_comm;
	for (int n = 0; n< arr.length(); n++)
	{
		//str_comm += QString::number(arr[n],16) + " ";
		str_comm += QString("%1 ").arg((unsigned char)arr[n], 2, 16, QLatin1Char('0'));
	}
	str_comm += "\r\n";
	comm_text = node.firstChildElement("CmdName").text();
	port->write(arr);

}

void FLXMLCommandManager::excute_most_cmd(QString xml, QString cmd, QSerialPort*port)
{
	QString xml_path = QApplication::applicationDirPath() + xml;
	
	QFile xml_file(xml_path);
	QDomDocument dom;
	if (!xml_file.open(QIODevice::ReadOnly))
		return;

	if (!dom.setContent(&xml_file)) 
	{
		xml_file.close();
		return;
	}

	xml_file.close();

	if (dom.isNull() || (!dom.isDocument()))
	{
		qDebug() << "QxPluginServer::parseManifest: Failed to parse the manifest file!";
	}

	//QDomNodeList pluginNodes = dom.elementsByTagName("plugin");
	QDomElement docElem = dom.documentElement();//zigbee

	auto node = docElem.firstChild();//Settings
	node = node.firstChild();//defaltConmannd

	while (node.isNull() == false)
	{
		QDomElement e = node.toElement(); // try to convert the node to an element.
		if (!e.isNull()) {
			qDebug() << qPrintable(e.tagName()); // the node really is an element.

			auto child = node.firstChild();
			while (child.isNull() == false)
			{
				if (child.toElement().tagName() == "MacroCmd")
				{
					excute_cmd(child, port);
				}
				else if (child.toElement().tagName() == "MacroScript")
				{
					auto marco_child = child.firstChild();//name
					marco_child = marco_child.nextSibling();
					while (marco_child.isNull() == false)
					{
						if (marco_child.toElement().tagName() == "MacroCmd")
						{
							auto child = marco_child.firstChild();
							while (child.isNull() == false)
							{
								auto cmd_name = child.toElement().text();
								auto node = GetZigBeeProCMD()->find_node_byName(cmd_name);
								if (node.isNull() == false)
									excute_script_cmd(node,port);
								child = child.nextSibling();
							}

						
						}
						marco_child = marco_child.nextSibling();
					}
				}
				child = child.nextSibling();
			}

		}
		node = node.nextSibling();
	}
}

void FLXMLCommandManager::excute_cmd(QDomNode node, QSerialPort* port)
{
	auto comm_text = node.firstChildElement("CmdHeader").text();
	auto head_list = comm_text.split(" ");

	bool bOk;
	QByteArray arr;
	//header
	char TX = 0x02;
	arr.append(TX);
	foreach(auto str, head_list)
	{
		int hex = str.toInt(&bOk, 16);
		arr.append(hex);
	}
	arr.append('0');
	
	//parameter;
	auto params = node.firstChildElement("CmdParms");
	auto param_node = params.firstChild();

	while (param_node.isNull() == false)
	{
		auto param_element = param_node.toElement();
		//default value ;
		QString default_value = param_element.firstChildElement("ParmDefaultValue").text();
		auto param_name = param_element.firstChildElement("ParmName").text();
		if (default_value.isEmpty())
			default_value = "00";
		auto type_node = param_element.firstChildElement("ParmType");

		// 		if(type_node.text() == "tEnum")
		// 		{
		// 		}
		// 		else if(type_node.text() == "tEdit" )
		// 		{			
		// 		}
		// 		else if(type_node.text() == "tInt")
		// 		{
		// 		}
		// 		else if(type_node.text() == "tBool")
		// 		{
		// 		}
		// 		else 
		if (type_node.text() == "tBitArray")
		{

		}
		// 		else if(type_node.text() == "tArray")
		// 		{
		// 			
		// 		}
		else if (type_node.text() == "tSelect")
		{

		}


		auto param_list = default_value.split(" ");
		foreach(auto param, param_list)
		{
			int hex = param.toInt(&bOk, 16);
			arr.append(hex);
		}

		param_node = param_node.nextSibling();

	}


	char ch_len = arr.length() - 4;
	arr[3] = ch_len;

	arr.append(Checksum(arr, arr.length()));

	QString str_comm;
	for (int n = 0; n< arr.length(); n++)
	{
		//str_comm += QString::number(arr[n],16) + " ";
		str_comm += QString("%1 ").arg((unsigned char)arr[n], 2, 16, QLatin1Char('0'));
	}
	str_comm += "\r\n";
	comm_text = node.firstChildElement("CmdName").text();

	
	port->write(arr);

}

void FLXMLCommandManager::excute_script_cmd(QDomNode script_node, QSerialPort* port)
{
	auto name = script_node.toElement().text();
	auto name_list = name.split(" ");
	auto node = GetZigBeeProCMD()->find_node_byName(name_list[0]);

	if (node.isNull() == false)
	{
		auto comm_text = node.firstChildElement("CmdHeader").text();
		auto head_list = comm_text.split(" ");

	

		bool bOk;
		QByteArray arr;
		//header
		char TX = 0x02;
		arr.append(TX);
		foreach(auto str, head_list)
		{
			int hex = str.toInt(&bOk, 16);
			arr.append(hex);
		}
		arr.append('0');

		

		int number_value = name_list.count();
		for (int n = 1; n < number_value; n++)
		{
			auto value = name_list[n];
			if (value.startsWith("0x", Qt::CaseInsensitive))
				value = value.right(value.length() - 2);

			while (value.length() >= 2)
			{
				QString right = value.right(2);
				int hex = right.toInt(&bOk, 16);
				arr.append(hex);
				value = value.left(value.length() - 2);
			}		

		}

		char ch_len = arr.length() - 4;
		arr[3] = ch_len;
	
		arr.append(Checksum(arr, arr.length()));
		port->write(arr);
	}
}