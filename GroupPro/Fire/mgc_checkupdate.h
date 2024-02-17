#ifndef QX_CHECKUPDATE_H
#define QX_CHECKUPDATE_H

#include <QObject>
#include <QWebEngineView>

class MGC_CheckUpdate : public QObject
{
	Q_OBJECT

public:
	static MGC_CheckUpdate* GetInstance();
	~MGC_CheckUpdate();

public slots:
	void check_update(bool bShow);
	void web_loadFinished(bool ok);
private:
	MGC_CheckUpdate(QObject *parent);
	void GetString(const QString& str);
	static MGC_CheckUpdate* instance;
	bool _show_no_newversiondialog;
	QWebEngineView * web;
};

#endif // QX_CHECKUPDATE_H
