#include "PCAPReader.h"
#include "pcap.h"
#include <QFile>
#include <QMessageBox>


// PCAPReader/PCAPReader.cpp
void PCAPReader::run()
{
     readPCAP(mFileName);
}


void PCAPReader::readPCAP(QString filename)
{
	pcap_hdr_t pcap_hdr;

	pcaprec_hdr_t pcaprec_hdr;

	mac_hdr_t *mac_hdr;

	qint64 bufSize = 1024;
	char *buf = new char[bufSize];
	ZBInt32 lmsec, delta;
	ZBTime  lzt;
	qint64 dataSize;
	int j = 0;
	

	//QString str_file_name = QFileDialog::getOpenFileName(this,"Choose a Job ", str);
	//if(str_file_name.isNull())
	//	return;

	//QString fileName = QApplication::applicationDirPath()+"/"+filename;
	QString fileName = filename;
	QFile fileToRead(fileName);
   
    fileToRead.open(QIODevice::ReadOnly);
	
	fileToRead.read((char*)&pcap_hdr, sizeof(pcap_hdr_t));
		
	if (pcap_hdr.magic_number == 0xd4c3b2a1 || pcap_hdr.magic_number == 0xa1b2c3d4) 
	{		
		while (!fileToRead.atEnd()) 
		{
			sync.lock();
			if (mPause)
				pauseCond.wait(&sync); // in this place, your thread will stop to execute until someone calls resume
			sync.unlock();
			
			fileToRead.read((char*)&pcaprec_hdr, sizeof(pcaprec_hdr_t));
				
			int len;
			ZBTime zt;
		
			if (pcap_hdr.magic_number == 0xd4c3b2a1) 
			{
				len = qToBigEndian(pcaprec_hdr.incl_len);
				zt = ZBTime(qToBigEndian(pcaprec_hdr.ts_sec), qToBigEndian(pcaprec_hdr.ts_usec));
			}

			else
			{
				len = (pcaprec_hdr.incl_len);
				zt = ZBTime((pcaprec_hdr.ts_sec), (pcaprec_hdr.ts_usec));
			}

			
			if (j != 0) {
				delta =( (zt.sec - lzt.sec)*1000000+(zt.msec - lzt.msec)) / 1000;
				//qDebug() << j <<" Sleeping for " << delta << " msec";
				if (j == 1206) 
					j = j;

				if( delta< 5000)
					msleep(delta);
			}
				

			lmsec = zt.msec;
			lzt = zt;

			QByteArray arr = fileToRead.read(len);
			
			emit sendPacket(arr, zt);
			//qDebug() << "emit sendPacket(arr, zt)";
						
			j++;
		}

		
	}
	else
	{
		//QMessageBox::critical(this, "File read error", tr("Invalid pcap file: <b>%1</b>").arg(filename));
	}

	//dataStreamReader >> pcap_hdr.magic_number >> pcap_hdr.version_major >> pcap_hdr.version_minor >> pcap_hdr.thiszone >> pcap_hdr.sigfigs >>pcap_hdr.snaplen >> pcap_hdr.network;
    fileToRead.close();

	

	emit Finished(j);

}