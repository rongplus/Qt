#include "MGC_CheckUpdate.h"

#include <QProcess>

#include <QDomDocument>
#include <QDialog>
#include <QTextStream>
#include <QWebEngineView>
#include "mgc_upgradefiledialog.h"
#include <QTextEdit>

#define Version   "1.0.6.2"


MGC_CheckUpdate* MGC_CheckUpdate::instance = nullptr;
MGC_CheckUpdate::MGC_CheckUpdate(QObject *parent)
	: QObject(parent)
{
	web = new QWebEngineView(0);
}

MGC_CheckUpdate::~MGC_CheckUpdate()
{
	if (web)
		delete web;
}


void MGC_CheckUpdate::check_update(bool bShow)
{
	_show_no_newversiondialog = bShow;
	
	web->load(QUrl("https://www.mircom.com/mircom.com/eng/Manifest_firelink.xml"));

	connect(web,SIGNAL(loadFinished(bool )),this, SLOT(web_loadFinished(bool)));
}
void MGC_CheckUpdate::GetString(const QString& str)
{
	QDomDocument dom;

	if (!dom.setContent(str))
	{
		return;
	}
	auto item = dom.elementsByTagName("Version");
	QString ver = Version;

	QString ver_server = item.item(0).toElement().text();
	item = dom.elementsByTagName("InfoUrl");
	auto url = item.item(0).toElement().text();;
	if (ver_server.compare(ver)>0)
	{
		UpgradeFileDialog dlg(nullptr);
		dlg.set_newversion(ver_server,url);
		dlg.exec();
		return;
	}
	if (_show_no_newversiondialog)
	{

	}
}
void MGC_CheckUpdate::web_loadFinished(bool ok)
{
	if(ok == false)
		return;
	QWebEngineView * web = (QWebEngineView *)sender();
	// web->page()->toHtml(GetString(QString& str));
	QWebEnginePage *page = web->page();
	//QTextEdit *textEdit = new QTextEdit;
	// *textEdit must remain valid until the lambda function is called.
	//page->toPlainText toHtml([textEdit](const QString &result) { textEdit->setPlainText(result); });
	page->toHtml([this](const QString &result) { GetString(result); });

	
}

MGC_CheckUpdate* MGC_CheckUpdate::GetInstance()
{
	if(instance == nullptr)
		instance = new MGC_CheckUpdate(0);
	return instance;
}