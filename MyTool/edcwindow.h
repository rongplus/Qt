#ifndef EDCWINDOW_H
#define EDCWINDOW_H

#include <QMainWindow>

namespace Ui {
class EDCWindow;
}

class EDCWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit EDCWindow(QWidget *parent = 0);
	~EDCWindow();

private:
	Ui::EDCWindow *ui;
};

#endif // EDCWINDOW_H
