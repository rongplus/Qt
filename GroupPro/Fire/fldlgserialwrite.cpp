#include "fldlgserialwrite.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
#include "flzcserialport.h"
#include "qx_xmodem.h"
#include <QDebug>
#include <QFileDialog>;
#include <QFile>
#include "FLData.pb.h"
#include "zc.pb.h"
#include "fljobprotol.h"
#include "flglobal.h"
#include <iostream>
#include <fstream>
#include "flxmlcommandmanager.h"


#define NLME_JOIN 0x9746
#define NLDE_DATA 0x9B40
using namespace std;
FLDlgSerialWrite::FLDlgSerialWrite(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(On_OK()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(On_Cancel()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(on_login()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(on_sendconfig()));
	connect(ui.widget, SIGNAL(state_change(int)), this, SLOT(on_state_changed(int)));

	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(on_getconfig()));
	connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(on_filename()));
	
	// fill infomation

	serial = nullptr;

	_data = nullptr;
	_type = 0;
	_zc = nullptr;
	connect(&_timer, SIGNAL(timeout()), this, SLOT(update_state()));
	_timer.start(1000);
	_zcport = new FLZCSerialPort();

}

FLDlgSerialWrite::~FLDlgSerialWrite()
{

}

void FLDlgSerialWrite::on_login()
{
	_password = ui.lineEdit->text();
	if (_password.isEmpty())
	{
		QMessageBox::warning(this, "Please input password.", "Password can not be null.");
		return;
	}	

	QByteArray arr;
	arr.append('\r');
	serial->write(arr);
	serial->disconnect();
	connect(serial, SIGNAL(readyRead()), this, SLOT(on_login_data()));
	arr.clear();
	arr.append("login");
	ui.label_login->setText("Login...");
	_out_string = "Password";
		
	arr.append('\r');
	serial->write(arr);
	_try_times = 20;
}

void FLDlgSerialWrite::on_sendconfig()
{

	QByteArray data;

	{
		Q_ASSERT(_zc);
		auto zone_control = _zc;
		ZC *zc = new ZC();
		zone_control->writeFirmware(zc);
		int size = zc->ByteSize();
		char *buffer = new char[size];
		zc->SerializeToArray(buffer, size);
		//data.fromRawData(buffer, size);
		for (int n = 0; n < size; n++)
			data.append(buffer[n]);

		//ui.label_login->setText("Send data...");
		delete zc;
		delete[] buffer;
	}
	

	ui.label_login->setText("Send data...");

	FLZCSerialPort port;
	port.set_messagelable(ui.label_2);

	serial->disconnect();

	

	connect(&port, SIGNAL(message(QByteArray)), this, SLOT(on_showmessage(QByteArray)));

	FLZCSerialPort::TransitState ret = port.send_config(serial, data,1024);
	if (ret != FLZCSerialPort::Successed)
	{
		QMessageBox::warning(this, tr("Write config"), "send config failed");
	}
	else
	{
		QMessageBox::warning(this, tr("Write config"), "send config sucessful");
	}

	ui.label_login->setText("Finished sending");
	
}

void FLDlgSerialWrite::on_getconfig()
{
	if (ui.checkBox->isChecked())
	{
		if (ui.label_3->text() == "")
		{
			QMessageBox::warning(this, "Please select a file", "File name should not be empty");
			return;
		}
	}
	QString filename = "zc_save.cfg";
	if (ui.checkBox->isChecked())
	{
		filename = ui.label_3->text();		
	}

#if 1
	
	QByteArray arr = "get config";	
	auto ret = _zcport->xmodemReceive(serial, arr, 1, filename);

	if (ret == 0)
	{
		if (ui.checkBox_2->isChecked())
		{
			FLJobProtol::ZCFromFile(_zc, filename);	
			emit updata_zc(_zc);
		}
		if (ui.checkBox->isChecked())
			QMessageBox::warning(this, "Success!", "Jos is saved to " + filename);

		QMessageBox::warning(this, "Success!", "ZC is updated");
	}
	else
		QMessageBox::warning(this, "Failed!", "Jos is not saved to job.bin");
#endif
#if 0
	_zc_data.clear();
	serial->disconnect();
	connect(serial, SIGNAL(readyRead()), this, SLOT(on_getconfig_data()));
	QByteArray out_arr("get config");
	out_arr.append('\r');
	serial->write(out_arr);

	if (serial->waitForBytesWritten(1000) == false)
	{
		QMessageBox::warning(this, "Failed!", "Jos is not saved to job.bin");
		return;
	}
	if (serial->waitForReadyRead(1000) == false)
	{
		QMessageBox::warning(this, "Failed!", "Jos is not saved to job.bin");
		return;
	}
	QByteArray arr = serial->readAll();
	if (arr.contains("OK"))
	{
		int nRet = receive(0);
		if (nRet < 0)
		{
			QMessageBox::warning(this, "Failed!", "Could not get ZC config");
			return;			
		}
		else
	

#endif

}

void FLDlgSerialWrite::onReadyRead_zc()
{	
	QByteArray arr = serial->readAll();
	qDebug() << _out_string << "== " << arr;
	ui.label_2->setText(arr);	
}

void FLDlgSerialWrite::on_login_data()
{
	static int state = 0;
	QByteArray arr = serial->readAll();
	qDebug() << _out_string << "== " << arr;
	ui.label_2->setText(arr);

	if (_out_string == "Password")
	{
		if (arr.endsWith(_out_string + ":") == true)
		{
		
			QByteArray o_arr;
			o_arr.append(_password);
			o_arr.append('\r');
			serial->write(o_arr);
			_out_string = "Success";
			qDebug() << "send passowrd == " << o_arr;
			_try_times = 20;
		}
		else if (arr.contains("Root screen") && state == 0)
		{
			state = 1;
		}
		else if (arr.contains("ERROR") && state == 1)
		{
			QByteArray o_arr("\rlogin");
			o_arr.append('\r');
			qDebug() << "send login == " << o_arr;
			serial->write(o_arr);
			state = 0;
		}

	}
	else if (_out_string == "Success" && arr.contains(_out_string) == true)
	{
		ui.label_login->setText("Login Successful...\n Send/get config is enabled.");	
		ui.groupBox->setEnabled(true);
		serial->disconnect();
	}
}

void FLDlgSerialWrite::on_filename()
{
	auto name = QFileDialog::getSaveFileName(this, "save file as", "", "CFG Files (*.cfg)");
	if (name == "")
		return;
	ui.label_3->setText(name);
}

void FLDlgSerialWrite::On_OK()
{
	QDialog::accept();
}

void FLDlgSerialWrite::On_Cancel()
{
	QByteArray o_arr;
	if (serial)
	{
		o_arr.append(CAN);
		serial->write(o_arr);
		serial->write(o_arr);
		serial->write(o_arr);
	}


	QDialog::reject();
}

void FLDlgSerialWrite::set_zc(FLZoneControllor* zc1)
{
	_zc = zc1;	
	setWindowTitle(zc1->getProperty("Name").value.toString() + " Configuration");
}

void FLDlgSerialWrite::on_getconfig_data()
{
	
}

void FLDlgSerialWrite::on_sendconfig_data()
{
}

void FLDlgSerialWrite::on_zigbee_data()
{
}

void FLDlgSerialWrite::on_showmessage(QByteArray arr)
{
	ui.label_2->setText(arr);
}

void FLDlgSerialWrite::update_state()
{
	ui.label_2->setText("");
}

void FLDlgSerialWrite::onReadyRead_OTA()
{
	QByteArray arr = serial->readAll();
	qDebug() << _out_string << "== " << arr;
	ui.label_2->setText(arr);
	
	
	//get complete data;
	if ((unsigned int)arr[0] == ZTC_STX)
	{
		_zc_data.clear();
	}

	_zc_data.append(arr);
	if (_zc_data.length()<4)
		return;
	remain_data_size = arr[3];
	if (_zc_data.length() < remain_data_size - ZTC_CRC_LENGH)
		return;
	int cmd = parse_rec_data(_zc_data);

	switch (cmd) 
	{
	case NLME_JOIN:
		//login;
		//SendData("login 3333");
		break;
	case NLDE_DATA:
		//disconnet 
		//	SendData(Packets 1);
		break;
	default:
		qDebug() << "cmd"<<cmd;
			break;
	}

}

int FLDlgSerialWrite::parse_rec_data(QByteArray arr)
{
	auto s1 = R"(This is a "raw" string)";


	int par_len = arr[3];
	int head_count = 4;
	while (arr.length() >= par_len + head_count + ZTC_CRC_LENGH  && par_len >= 0)
	{		
		if ((int)arr[1] == 0x97 && (int)arr[2] == 0x46)
		{
			//send data// 
			//if get success. then send login
			
			//wait for "OK" in NLDE_DATA;
			//get ok //
		
			return NLME_JOIN;
		}

		if ((int)arr[1] == 0x9B && (int)arr[2] == 0x40)
		{
			// get ok
			//send data// 
			//
			//while (packaget)
			{
			//	SendData(Packets 1);
				//wait for ACT;
			}
			// get

			
			return NLDE_DATA;
		}

		arr = arr.right(arr.length() - par_len - head_count - ZTC_CRC_LENGH);
		par_len = arr[3];
	}

	return 0;

}

void FLDlgSerialWrite::set_datatype(int type)
{
	_type = type;
}

void FLDlgSerialWrite::set_data(QByteArray & data)
{
	_data = &data;
}

void FLDlgSerialWrite::on_state_changed(int state)
{
	if (state == 1)
	{
		serial = ui.widget->get_port();
		ui.groupBox_2->setEnabled(true);
		
		QByteArray arr;
		arr.append('\r');
		serial->write(arr);

		connect(serial, SIGNAL(readyRead()), this, SLOT(onReadyRead_zc()));

		ui.groupBox_2->setEnabled(true);
		ui.pushButton_3->setEnabled(true);

		ui.label_login->setText("Connet to port,Please login");

	}
	else
	{
		serial = nullptr;
	}

}

void FLDlgSerialWrite::send_zc_data()
{
	// excute script
	//send config
}

void FLDlgSerialWrite::send_wio_data()
{
}

int FLDlgSerialWrite::receive(unsigned char *dest)
{
	int package_len = 1029;
	QByteArray buff1024;
	pakager_serials = 1;
	
	int waittimes = 20;
	int c_times = 0;
	bool start_tranfer = false;
	while (true)
	{		
		if (start_tranfer == false)
		{
			c_times++;
			if (c_times<16)
			{
				if (_outbyte('C', 20) == false)
				{
					return -1;
				}
			}
			else
				return -1;
		}

		if (serial->waitForReadyRead(1000) == false)
		{
			waittimes--;
			if (waittimes<0)
				return -2;
			continue;
		}
		waittimes = 20;

		QByteArray arr = serial->readAll();
		qDebug() << "data length = " +  QString::number(arr.length()) + "data = " + arr ;
		for (int n = 0; n< arr.length(); n++)
		{
			int c = arr[n];
			if (c == EOT)
			{
				qDebug() << "Get EOT " << arr.length();
				save_data(buff1024);			
				return 1;
			}
			else if (c == CAN)
			{
				qDebug() << "Get CAN " << arr.length();
				break;
			}
			else if (c == STX)
			{
				start_tranfer = true;
				buff1024.append(arr.left(n));
				//qDebug() << arr;
				if (save_data(buff1024) == false)
				{
					return -3;
				}
				buff1024.clear();

				buff1024.append(arr.mid(n, arr.length()));
				int waittimes = 20;

				while (buff1024.size() < package_len)
				{
					if (serial->waitForReadyRead(1000) == false)
					{
						waittimes--;
						if (waittimes<0)
							return -1;
						continue;
					}
					waittimes = 20;

					QByteArray buff = serial->readAll();
					buff1024.append(buff);
				}
				if (save_data(buff1024) == false)
				{
					return -3;
				}			
				break;
			
			}
			else if (c < 0)
			{
				buff1024.clear();
				if (_outbyte(ACK, 20) == false)
				{
					return -1;
				}
				break;
			}
		}
	}
	save_data(buff1024);
	return 0;
}

bool FLDlgSerialWrite::save_data( QByteArray& buff1029)
{
	int data_len = 1024;
	int package_len = 1029;
	XMODEM dm(0, 0);
	while (buff1029.length() >= package_len)
	{
		unsigned char crc = buff1029[data_len + 3];
		char * ppp = (char *)buff1029.data();
		QByteArray arr = buff1029.left(data_len + 3);
	

		if ((unsigned int)arr[0] != STX || pakager_serials != (unsigned int)arr[1] || ((unsigned char)arr[1] != (unsigned char)(~arr[2])))
		{
			_outbyte(NAK, 50);
		}
		else
		{
			arr = arr.right(data_len);
			unsigned short data_crc = dm.crc16_ccitt((unsigned char*)arr.constData(), arr.length());
			int crc2 = (unsigned short)(data_crc >> 8) & 0xFF;

			if (crc2 != crc)
			{
				qDebug() << "crc  data_crc = " << crc << crc2;
			}
			else
			{
				retry_number = 25;				
				buff1029 = buff1029.right(buff1029.length() - package_len);
				_zc_data.append(arr);
				pakager_serials++;
				_outbyte(ACK, 50);
			}
		}

		retry_number--;
		if (retry_number < 0)
			return false;
	}
	return true;
}

bool FLDlgSerialWrite::save_data(QDataStream* datasteam, QByteArray& buff1029)
{
	int data_len = 1024;
	int package_len = 1029;
	XMODEM dm(0, 0);
	while (buff1029.length() >= package_len)
	{
		unsigned char crc = buff1029[data_len + 3];
		char * ppp = (char *)buff1029.data();
		QByteArray arr = buff1029.left(data_len + 3);
		//qDebug() << "-->data = " << arr;

		if ((unsigned int)arr[0] != STX || pakager_serials != (unsigned int)arr[1] || ((unsigned char)arr[1] != (unsigned char)(~arr[2])))
		{
			//emit message (QString("Get Wrong data: head = %1 package number = %2 package number=%3" ).arg((unsigned int)arr[0]).arg((unsigned int)arr[1] ).arg( pakager_serials));
			//qDebug() << "Wrong data: head = " <<(unsigned int)arr[0] << "package number " <<(unsigned int)arr[1] << "!= " << pakager_serials;			
			//buff1024.clear();
			_outbyte(NAK, 50);
		}
		else
		{
			arr = arr.right(data_len);
			unsigned short data_crc = dm.crc16_ccitt((unsigned char*)arr.constData(), arr.length());
			int crc2 = (unsigned short)(data_crc >> 8) & 0xFF;

			if (crc2 != crc)
			{
				//buff1024.clear();
				//_outbyte(NAK,50);	
				qDebug() << "crc  data_crc = " << crc << crc2;
			}
			else
			{ 
				retry_number = 25;
				datasteam->writeRawData(arr.constData(), arr.length());
				buff1029 = buff1029.right(buff1029.length() - package_len);
				//_zc_data.append(arr);
				pakager_serials++;			
				_outbyte(ACK, 50);
			}
		}

		retry_number--;
		if (retry_number < 0)
			return false;
	}
	return true;
}

bool FLDlgSerialWrite::_outbyte(int c, int times)
{
	QByteArray ba;
	ba.append(QChar(c));	
	for (int retry = 0; retry < times; ++retry)
	{	
		serial->write(ba);
		if (serial->waitForBytesWritten(1000) == true)
			return true;	
	}

	return false;

}