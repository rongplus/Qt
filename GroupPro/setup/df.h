#ifndef DF_H
#define DF_H

#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui {
class DF;
}

class DF : public QMainWindow
{
	Q_OBJECT

public:
	explicit DF(QWidget *parent = 0);
	~DF();
private slots:
	void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
	void on_btnAdd1_clicked();
	void on_btnAdd2_clicked();
private:
	Ui::DF *ui;
};

#endif // DF_H
