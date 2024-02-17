#ifndef DBCONNECT_H
#define DBCONNECT_H
#include <QObject>
#include <QDebug>
#include <QString>
#include <QSslConfiguration>
#include <QVariant>
#include <QJsonValueRef>

#include "webrequest.h"


class JsonParsingForApp: public QObject{

    Q_OBJECT

public:
    void constructModuleListForPlate(QString myJStr="");

    // for plates operation (setup)
    QJsonArray getPlatesFromSetupStr(QString myJStr="");

    // for modules operation (setup)
    QJsonValue getOnePlateFromPlates(QJsonArray obj, QString plateNum);
    QStringList getModulesNameListFromPlateJobj(QJsonValue value);
    QString getFieldsRefJosonStrForOneModule(QJsonValue value, QString moduleNum);

    void inline setJsonStr(QString s){myJstr = s;}
private:
    QString myJstr;


};

class DBConnect : public QObject {

    Q_OBJECT
  //  Q_PROPERTY(QString responseStr author READ author WRITE setAuthor)

public:
    DBConnect();
    bool isConnectedToDB() { return isconnected; }
    QString responseStr;
    Q_INVOKABLE void mergeKeyVisitsData(int site,QString);
    Q_INVOKABLE QStringList getModuleListsFromOnePlate(QString platNum);
    Q_INVOKABLE QString getFieldsListFromOneModule(QString platNum, QString moduleNum);
    //Q_INVOKABLE QString createRadioListJson(QStringList elements);

public slots:
    bool login(const QString &dfserver, const QString &dfuser, const QString &dfpass);
    bool logout();
    bool getStudies();
    bool setStudyNumber(uint);
    bool getSitesInfoList();
    bool getSubjectsInfoList(int);
    bool getVisitsMap();
    bool getKeys(int);
    bool getSetup();

signals:
    void connected(const QString& result);
    void failure(const QString& result);
    void studiesList(const QString& result);
    void sitesInfoList(const QString & result);
    void subjectsInfoList(const QString& result);
    void visitsMap(const QString& result);
    void logoutInfo(const QString& result);

private:
    QString uid;
    //QString responseStr;
    WebRequest req;
    bool isconnected;
    JsonParsingForApp jparse;
};


#endif // DBCONNECT_H
