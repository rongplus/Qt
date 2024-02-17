#ifndef QX_UPGRADEFILEDIALOG_H
#define QX_UPGRADEFILEDIALOG_H

#include <QDialog>
#include "ui_qx_upgradefiledialog.h"

class UpgradeFileDialog : public QDialog
{
	Q_OBJECT

public:
	UpgradeFileDialog(QWidget *parent = 0);
	~UpgradeFileDialog();

	void set_newversion(QString version,QString url);
protected slots:
	void onClose();
private:
	Ui::UpgradeFileDialog ui;
};

#endif // QX_UPGRADEFILEDIALOG_H
