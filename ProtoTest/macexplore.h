#ifndef MACEXPLORE_H
#define MACEXPLORE_H

#include <QDialog>

namespace Ui {
class MacExplore;
}

class MacExplore : public QDialog
{
	Q_OBJECT

public:
	explicit MacExplore(QWidget *parent = 0);
	~MacExplore();

private:
	Ui::MacExplore *ui;
};

#endif // MACEXPLORE_H
