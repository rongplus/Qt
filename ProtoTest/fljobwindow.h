#ifndef FLJOBWINDOW_H
#define FLJOBWINDOW_H

#include <QMainWindow>
#include "ui_fljobwindow.h"
#include "fltreewidget.h"
#include "flpropertytable.h"
#include "FLData.pb.h"
#include "flzonecontrollor.h"
#include "flwio.h"
#include "flfloorview.h"



class FLJobWindow : public QMainWindow
{
	Q_OBJECT

public:
	FLJobWindow(QString filename="",QWidget *parent = 0);
	~FLJobWindow();

public slots:
	///file menu
	void on_new_job();
	void on_save_job();
	void on_save_job_as();
	void on_delete_job();
	void on_open_job();
	void on_close_job();

	void on_job_setting();
	///view menu
	void on_view_floor();
	////edit menu	
	void on_edit_add();
	void on_edit_edit();
	void on_edit_delete();
	void on_write_hardware();

	void on_res_DockProperty();
public:
	QToolBar* GetMainToolbar();
	void openjob(QString filename);
	void openjob(FLJob* job);
protected slots:
	///tree
	void on_jobtree_clicked(QTreeWidgetItem * , int );
	void on_jobtree_doubleClicked(QTreeWidgetItem * , int );
	void on_jobtree_menu(QTreeWidgetItem * , QPoint );

	void TabCloseRequested(int index);
	///interaction
	void UpdateItem(QGraphicsItem* item);
	void View_Item_changed(FLObject* obj);

	void on_update_zc(FLZoneControllor*);
	
private:

	void Init();
	void open_floor(FLFloor* floor);
	void excute_job_menu(QTreeWidgetItem *, FLObject* obj, QPoint pt);
	void excute_floor_menu(QTreeWidgetItem *, FLObject* obj, QPoint pt);
	void excute_zc_menu(QTreeWidgetItem *, FLObject* obj, QPoint pt);
	void excute_wio_menu(QTreeWidgetItem *, FLObject* obj, QPoint pt);

	void write_zc(FLZoneControllor* flzc);
	FLFloorView* visual_floor(FLFloor* floor);
	int visual_floor_index(FLFloor* floor);
	

	FLTreeWidget* m_pJobTree;
	FLPropertyTable* m_pProTable;
	
signals:
	void wirelessMonitor(QString);
	void networkMonitor(QString, int);
	void zigBeeTerminal(QString);
	void usbTerminal(QString);

	void createJob(void);
	//void openJob(void);
	void openJob(QString);
	void openSS(QString);
private:
	Ui::FLJobWindow ui;
};

#endif // FLJOBWINDOW_H
