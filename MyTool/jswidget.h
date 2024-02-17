#ifndef JSWIDGET_H
#define JSWIDGET_H

#include <QWidget>

namespace Ui {
class JSWidget;
}

class JSWidget : public QWidget
{
	Q_OBJECT

public:
	explicit JSWidget(QWidget *parent = 0);
	~JSWidget();
protected slots:
	void on_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());
private:
	QString exePath();
	Ui::JSWidget *ui;
};

#endif // JSWIDGET_H
