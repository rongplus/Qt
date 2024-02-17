#include "flfloorpropertydlg.h"

FLFloorPropertyDLG::FLFloorPropertyDLG(FLFloor* pFloor, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	_floor = pFloor;
	Q_ASSERT(_floor);
	ui.lineEdit->setText(_floor->getProperty("Name").value.toString());
	ui.lineEdit_2->setText(_floor->getProperty("Comments").value.toString());
	ui.lineEdit_3->setText(_floor->getProperty("FloorGraph").value.toString());
	ui.comboBox->addItems( QStringList() << "Meters" << "Yards" << "Feet" << "Inches" << "Centimeters");
	ui.comboBox->setCurrentText(_floor->getProperty("unit_of_measure").value.toString());

	connect(ui.pb_ok, SIGNAL(clicked()), this,  SLOT(on_ok()));
	connect(ui.pb_cancel, SIGNAL(clicked()), this, SLOT(on_cancel()));

}

FLFloorPropertyDLG::~FLFloorPropertyDLG()
{

}

void FLFloorPropertyDLG::on_cancel()
{
	QDialog::reject();
}

void FLFloorPropertyDLG::on_ok()
{	
	_floor->addProperty("Comments", ui.lineEdit_2->text());
	_floor->addProperty("FloorGraph", ui.lineEdit_3->text(), false);
	_floor->addProperty("Name", ui.lineEdit->text());	
	_floor->addProperty("unit-of-measure", ui.comboBox->currentText(), QStringList() << "Meters" << "Yards" << "Feet" << "Inches" << "Centimeters");
	QDialog::accept();
}