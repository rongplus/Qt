#ifndef FLWIDGETPCAPVIEW_H
#define FLWIDGETPCAPVIEW_H

#include <QWidget>
#include "ui_flwidgetpcapview.h"
#include "PCAPReader.h"

class FLWidgetPCAPView : public QWidget
{
	Q_OBJECT

public:
	FLWidgetPCAPView(QWidget *parent = 0);
	~FLWidgetPCAPView();
protected slots:
	void on_open_file();

	void on_analysis_file_start();
	void on_analusis_file_pause();

	void readFinished(int packets);
	void ProcessPacket(QByteArray arr, ZBTime zt);
private:
	Ui::FLWidgetPCAPView ui;
	SnifferMain *snifferMain;
	PCAPReader *reader;
};

#endif // FLWIDGETPCAPVIEW_H
