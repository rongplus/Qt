#ifndef FLWIDGETZIGBEENETWORKVIEW_H
#define FLWIDGETZIGBEENETWORKVIEW_H

#include <QWidget>
#include <QMutex>
#include <QTextStream>

#include "ui_flwidgetzigbeenetworkview.h"
#include "SnifferMain.h"


class FLWidgetZigbeeNetworkView : public QWidget
{
	Q_OBJECT

public:
	FLWidgetZigbeeNetworkView(QString panid ,QWidget *parent = 0);
	FLWidgetZigbeeNetworkView( QWidget *parent = 0);
	~FLWidgetZigbeeNetworkView();

	QString pan_id();
	void process_data(QList<QString> list, QString data);

	void set_auto_scroll(bool bauto) { scrollSwitch = bauto; }

	ZBInt32 addFrame(ZBByte type, const ZBBytes& bin);

	void set_floor(FLFloor* floor);
	void snapshot(QString name);

	FLZoneControllor* find_zc(QString mac);
protected slots:
	void selectedMACFrameItem(int row, int col);
	void on_clear_table();
	void on_create_job();

	void onSearch();
	void on_fitview();
private:	
	void init();
	void hideCoulmns(bool hide);
	void fill_table(QList<QString> list, QString data);
	void update_floor(QList<QString> list, QString data);

	void addMACFrameDecode(int idx);
	//void addMACFrameDecode(const MACFrame& frame, QTreeWidget* tree);
	
	FLWio* find_wio(QString mac1, QString mac2, FLFloor* fl);

	void clear_table();
	void set_item(int row, int col, QString text, QColor clr, QString lable);
	void add_col(int col, QString text);
	Ui::FLWidgetZigbeeNetworkView ui;

	QString _pan_id;
	QMutex _mutex;
	QStringList _wio_data;
	QStringList _wio_ids;
	QHash<QString, QString> eMacs;
	bool scrollSwitch;

	
	QFile _fi;
	QTextStream* _record;
	SnifferMain *snifferMain;

	SavedFrames* _save_data;

	std::unordered_map<ZBInt32, SavedFrames::Saved> frames;

	QList <FLWio*> _dongle_wio;
};

#endif // FLWIDGETZIGBEENETWORKVIEW_H
