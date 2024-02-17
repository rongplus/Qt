#ifndef FLWIDGETZIGBEENETWORKMONITOR_H
#define FLWIDGETZIGBEENETWORKMONITOR_H

#include <QWidget>
#include "ui_flwidgetzigbeenetworkmonitor.h"
#include "SnifferMain.h"
class SnifferMC1322x;
#include "flwidgetzigbeenetworkview.h"
#include <QMutex>


class FLWidgetZigbeeNetworkMonitor : public QWidget
{
	Q_OBJECT

public:
	FLWidgetZigbeeNetworkMonitor(QWidget *parent = 0);
	~FLWidgetZigbeeNetworkMonitor();

protected slots:
	void onzbScan(bool enable);
	void onLog(bool enable);
	void onScroll(bool enable);
	void receivedFrame(const QByteArray& frmBin);

	void ProcessPacket(QByteArray arr, ZBTime zt);
	void onOpenJob();
	void floorChanged(int index);

	void onSaveDir();
	void on_snapshot();
private:

	FLWidgetZigbeeNetworkView* getPanID(QString panID);


	Ui::FLWidgetZigbeeNetworkMonitor ui;

	SnifferMC1322x* sniffer;
	SnifferMain *snifferMain;
	QMutex _mutex;
	QString m_dirPath;
};

#endif // FLWIDGETZIGBEENETWORKMONITOR_H
