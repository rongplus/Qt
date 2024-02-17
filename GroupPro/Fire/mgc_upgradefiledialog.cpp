#include "mgc_upgradefiledialog.h"
#include <QGraphicsDropShadowEffect>

UpgradeFileDialog::UpgradeFileDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint|Qt::WindowTitleHint);

	int w = 398;
	int h = 100;

	ui.label->setText("<a href=\"https://www.mircom.com/mircom.com/eng/install.msi\">Version</a>");
	ui.label->setTextFormat(Qt::RichText);
	ui.label->setTextInteractionFlags(Qt::TextBrowserInteraction);
	ui.label->setOpenExternalLinks(true);
	
	//setGeometry(  (parent->width()-w)/2 , (parent->height()-h)/2,w,h);
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
	effect->setBlurRadius(10);
	//effect->setColor(Qt::white);
	setGraphicsEffect(effect);

	connect(ui.pushButton,SIGNAL(clicked()), this, SLOT(onClose()));
}

UpgradeFileDialog::~UpgradeFileDialog()
{

}

void UpgradeFileDialog::set_newversion(QString version,QString url)
{
	QString str = QString("<a href=\"%1\">Version: %2</a>").arg(url).arg( version);
	ui.label->setText(str);
	str = QString("A new version of MGC Firelink Configurator released, Version %1 is available at %2\n you can download this version using the link").arg(version).arg( url);
	ui.label_2->setText(str);
}

void UpgradeFileDialog::onClose()
{
	QDialog::reject();
}
