#ifndef SPLITPHOTODIALOG_H
#define SPLITPHOTODIALOG_H

#include <QDialog>

#include <QAbstractButton>

namespace Ui {
class SplitPhotoDialog;
}

class SplitPhotoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SplitPhotoDialog(QWidget *parent = 0);
	~SplitPhotoDialog();
protected:

private:
	Ui::SplitPhotoDialog *ui;

	// QWidget interface
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);

	void save_image();
	void save(QImage& img, QString file_name,  QRect rt_dest,QRect rt_source);

	bool isInLine(int value, int line);
	int getX(int value, float ratio);
	int getY(int value, float ratio);

	QList<QLine> v_line;
	QList<QLine> h_line;

	int h_select_line;
	int v_select_line;
private slots:
	void on_spinBox_valueChanged(int arg1);
	void on_spinBox_2_valueChanged(int arg1);
	void on_toolButton_clicked();
	void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // SPLITPHOTODIALOG_H
