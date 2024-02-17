#ifndef FLNETVIEW_H
#define FLNETVIEW_H

#include <QWidget>
#include <QSplitter>
#include <QTableWidget>
#include <QToolTip>
#include <QMutex>

#include "ui_flnetview.h"
#include "flfloorview.h"
#include "FLMessageView.h"
#include "SnifferMain.h"

class flnetview : public QWidget
{
	Q_OBJECT

public:
	flnetview(SnifferMain *snifferMain, QWidget *parent = 0);
	~flnetview();

	void setScroll(bool enable) { scrollSwitch = enable;  };
	void setFile(bool enable) { _b_fromfile = enable; };
	QString geteMac(QString src);
	QList<QString> getsData() { return sdata; };
	

	FLFloorView* getFloorView() { return pfloorView; };
	void insertItem(QList<QString> list, QString data);
	void clear_table();
	
	void print_table(QString htmlFileName);
	void set_table_rowcount(int nRow);

private slots:
	void resizeEvent(QResizeEvent * event);
	void onSearch();
	void hheaderclicked(int);
	void on_create_job();
	void on_clear_table();
private:	
	QTableWidget* getTable() { return ui.tableWidget; }

	int insertCell(QString text, QString type, int i, QColor col);
	void selectedMACFrameItem(int row, int col);
	void addMACFrameDecode(int idx);
	void hideCoulmns(bool hide);
	
	virtual void showEvent(QShowEvent *event);
	
	Ui::flnetview ui;
	QSplitter *sp;
	QSplitter *leftsp;
	QList<QString> nList;
	FLFloorView* pfloorView;

	SnifferMain *snifferMain;
	QList<QString> sdata;
	int itemIndex;
	bool scrollSwitch;
	bool _b_fromfile;

	QHash<QString, QString> eMacs;

	bool _bInit;
	QMutex _mutex;
};

#endif // FLNETVIEW_H
