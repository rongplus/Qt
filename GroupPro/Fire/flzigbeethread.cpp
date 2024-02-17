#include "flzigbeethread.h"
#include "global.h"
#include <QTime>
#include <QDebug>

FLZigbeeThread::FLZigbeeThread(QObject *parent)
	: QThread(parent)
{
	_exit = false;
}

FLZigbeeThread::~FLZigbeeThread()
{

}

void FLZigbeeThread::run()
{
	while (_exit == false)
	{
		if (m_serial->waitForReadyRead(50000))
		{
			QByteArray arr = m_serial->readAll();
			//get complete data;
			if ((unsigned int)arr[0] == ZTC_STX)
			{
				data_received.clear();
			}

			data_received.append(arr);
			if (data_received.length()<4)
				continue;
			remain_data_size = arr[3];
			if (data_received.length() < remain_data_size - ZTC_CRC_LENGH)
				continue;
			parse_data(data_received);
		}
		
	}
}

void FLZigbeeThread::exit_thread()
{
	_exit = true;
}

void FLZigbeeThread::reset(QSerialPort * port, FLXMLCommandManager * man)
{
	Q_ASSERT(port);
	m_serial = port;

	Q_ASSERT(man);
	ZigBeePro_cmd = man;

	_exit = false;
}

void FLZigbeeThread::parse_data(QByteArray arr)
{
	int par_len = arr[3];
	int head_count = 4;
	while (arr.length() >= par_len + head_count + ZTC_CRC_LENGH  && par_len >= 0)
	{	
		if (crc_check(arr, par_len + head_count - 1, (unsigned int)arr[par_len + head_count]) == false)
			qDebug() << "rec data " << "CRC wrong";
		QString str_comm;
		for (int n = 0; n< par_len + head_count + ZTC_CRC_LENGH; n++)
		{
			str_comm += QString("%1 ").arg((unsigned char)arr[n], 2, 16, QLatin1Char('0')).toUpper();
		}
		qDebug() << "rec data " << str_comm;
		
		auto rec_list = str_comm.split(" ");
	
		auto node = find_cmd(rec_list);
		if (node == -1)
			data2frame(rec_list, str_comm);
		else
			emit comm_msg(node, rec_list, str_comm);

		arr = arr.right(arr.length() - par_len - head_count - ZTC_CRC_LENGH);
		par_len = arr[3];
	}

}

void FLZigbeeThread::data2frame(QStringList lt,  QString cmd)
{
	if (lt.count() < 4)
		return;
	
	int par_len = lt[3].toInt();
	int head_count = 4;
	QStringList frm_list;
	auto tm = QTime::currentTime().toString(Qt::SystemLocaleDate);

	frm_list.clear();
	frm_list <</*QString::number(index) << */QTime::currentTime().toString(Qt::SystemLocaleDate);
	frm_list << "15" << lt[3] << "Zigbee";
	frm_list << "Layer";
	frm_list << cmd;

	emit data_msg(frm_list,cmd);


}

int FLZigbeeThread::find_cmd(QStringList lt)
{
	foreach(auto node, ZigBeePro_cmd->all_commmand())
	{
		auto str_head = node.firstChildElement("CmdHeader").text();
		auto head_list = str_head.split(" ");
		if (head_list.count() == 2)
		{
			if (lt[1] == head_list[0] && lt[2] == head_list[1])
			{
				return ZigBeePro_cmd->all_commmand().indexOf(node);
			}
		}
		
	}

	return -1;
}

