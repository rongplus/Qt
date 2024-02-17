#ifndef FLMESSAGEVIEW_H
#define FLMESSAGEVIEW_H
#include <QPushButton>
#include <QScrollArea>
#include "ui_flmessageview.h"

class FLMessageView : public QScrollArea
{
	Q_OBJECT

public:
	FLMessageView(QWidget *parent = 0);
	~FLMessageView();

private:
	void btnClick(QWidget *b);
	QPair<int,int> gridPosition(QWidget * widget);
	void showGrid();
	void ShowMessage();

	QList<QString> lblList;
	QList<QPushButton*> btnList;

	Ui::FLMessageView ui;
};

#endif // FLMESSAGEVIEW_H
