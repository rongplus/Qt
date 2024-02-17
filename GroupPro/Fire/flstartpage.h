#ifndef FLSTARTPAGE_H
#define FLSTARTPAGE_H

#include <QWidget>
#include <QScrollArea>
#include <QTimer>
#include <QMutex>

#include "ui_flstartpage.h"
#include "flowlayout.h"
class FLStartPage : public QWidget
{
	Q_OBJECT

public:
	FLStartPage(QWidget *parent = 0);
	~FLStartPage();

	void init();

signals:
		void wirelessMonitor(QString);
		void networkMonitor(QString,int);
		void zigBeeTerminal(QString);
		void usbTerminal(QString);

		void createJob(void);
		//void openJob(void);
		void openJob(QString);
		void openSS(QString);

		void ota();
private slots:
		void resizeEvent(QResizeEvent * event);
		void onWM(void);
		void onNM(void);
		void onZDT(void);
		void onUDT(void);
		void onCJ(void);
		void onOJ(void);
		void onSS();
		void onOTA();

		void on_jobitemDoubleClicked(QListWidgetItem *item);
		void on_usb_itemDoubleClicked(QListWidgetItem *item);
		void on_Zigbee_debug_itemDoubleClicked(QListWidgetItem *item);
		void on_zigbee_network_itemDoubleClicked(QListWidgetItem *item);
		void on_wireless_itemDoubleClicked(QListWidgetItem *item);
		void on_sit_survey_itemDoubleClicked(QListWidgetItem *item);

		void update_port();
private:
		
private:
	Ui::FLStartPage ui;
	FlowLayout *flowLayout;
	QScrollArea* scrollarea;
	QTimer _timer;
	QMutex _mutex;
};

#endif // FLSTARTPAGE_H
