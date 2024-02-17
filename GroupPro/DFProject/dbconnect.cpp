#include "dbconnect.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QHash>

QString JsonParsingForApp:: getFieldsRefJosonStrForOneModule(QJsonValue value, QString moduleNum){
    QJsonObject obj = value.toObject();
    QJsonArray modRefs = obj["moduleRefs"].toArray();

    foreach (const QJsonValue & mvalue, modRefs) {
        QJsonObject obj = mvalue.toObject();
        if(obj.toVariantHash()["number"].toDouble() == moduleNum.toDouble()){ // this module{
            return QJsonDocument::fromVariant(mvalue.toVariant()).toJson(QJsonDocument::Compact);
        }
    }
    return "";
}

QStringList JsonParsingForApp:: getModulesNameListFromPlateJobj(QJsonValue value){
    QJsonObject obj = value.toObject();
    QJsonArray modRefs = obj["moduleRefs"].toArray();
    QStringList result;

    foreach (const QJsonValue & value, modRefs) {
        QJsonObject obj = value.toObject();
        result.append(obj.toVariantHash()["name"].toString() + "," + obj.toVariantHash()["number"].toString());
//        qDebug() <<"jparse:" << obj.toVariantHash()["name"].toString();
    }
    return result;
}

QJsonValue JsonParsingForApp:: getOnePlateFromPlates(QJsonArray obj, QString plateNum){
    QJsonArray plateArray = obj; // get plates array
    QJsonValue ref;

    foreach (const QJsonValue & value, plateArray) {
        QJsonObject obj = value.toObject();
        if(obj["number"].toDouble() == plateNum.toDouble()){
            ref = value;
            break;
        }
        //qDebug() <<"jparse:" <<obj["description"].toString() << " " << obj["number"].toDouble() ;
    }
    return ref;
}

QJsonArray JsonParsingForApp:: getPlatesFromSetupStr(QString str){
    //set json string if there is one
    if(str != "")
        this->myJstr = str;

    // getRecord -> plates[] -> plates[].moduleRefs[] -> plates[].moduleRefs[].name/description/number
    // for fields in a module==>  plates[].moduleRefs[].fieldRefs[]
    QJsonDocument jsonResponse;
    QJsonParseError error;

    jsonResponse = QJsonDocument::fromJson(myJstr.toUtf8(),&error); // parsing
    return jsonResponse.object()["plates"].toArray();
}

void JsonParsingForApp::constructModuleListForPlate(QString str){
    //set json string if there is one
    if(str != "")
        this->myJstr = str;

    // getRecord -> plates[] -> plates[].moduleRefs[] -> plates[].moduleRefs[].name/description/number
    // for fields in a module==>  plates[].moduleRefs[].fieldRefs[]
    QJsonDocument jsonResponse;
    QJsonArray jsonSetupArray;
    QJsonParseError error;

    jsonResponse = QJsonDocument::fromJson(myJstr.toUtf8(),&error); // parsing
    jsonSetupArray= jsonResponse.object()["plates"].toArray();
//    qDebug() <<"jparse:" <<jsonSetupArray.toVariantList();
    foreach (const QJsonValue & value, jsonSetupArray) {
        QJsonObject obj = value.toObject();
//        qDebug() <<"jparse:" <<obj.toVariantHash()["description"] << " " << obj.toVariantHash()["number"] <<  obj.toVariantHash()["number"];
    }

}

//========================> DBConnect <=======================================

QString DBConnect::getFieldsListFromOneModule(QString platNum, QString moduleNum){
    return jparse.getFieldsRefJosonStrForOneModule(jparse.getOnePlateFromPlates(jparse.getPlatesFromSetupStr(), platNum), moduleNum);
}

QStringList DBConnect::getModuleListsFromOnePlate(QString platNum){
 return jparse.getModulesNameListFromPlateJobj(jparse.getOnePlateFromPlates(jparse.getPlatesFromSetupStr(), platNum));
}

DBConnect::DBConnect()
{
    isconnected = false;
}


bool DBConnect::getSubjectsInfoList(int centerID){
    QByteArray data;
    if (req.reqPtList(centerID,data))
    {
        //qDebug() <<"get subjects list in object" << data;
        responseStr = data;
        emit subjectsInfoList(responseStr);
        return true;
    }
    else
    {
        qDebug("Are we still logged in?");
        return false;
    }
}


bool DBConnect::setStudyNumber(uint snum){
    QByteArray data;
    qDebug() <<"setStudyNumber() sid:>>>>>>>>" <<snum;
    if (req.reqSetStudy(snum, data))
    {
        //qDebug() <<"get study list in object" << data;
        responseStr = data;

        //emit studiesList(responseStr);
        return true;
    }
    else
    {
        qDebug("setStudyNumber: Are we still logged in?");
        return false;
    }
}

bool DBConnect::getSitesInfoList(){
    QByteArray data;
    if (req.reqCenters(data))
    {
        //qDebug() <<"get sites list in object" << data;
        responseStr = data;
        emit sitesInfoList(responseStr);
        return true;
    }
    else
    {
        qDebug("Are we still logged in?");
        return false;
    }
}

bool DBConnect::getStudies()
{
    QByteArray data;
    if (req.reqStudyList(data))
    {
        responseStr = data;
        emit studiesList(responseStr);
        return true;
    }
    else
    {
        qDebug("Are we still logged in?");
        return false;
    }
}

bool DBConnect::logout()
{
    QByteArray data;
    if (req.reqLogout(data))
    {
        responseStr = data;
        emit logoutInfo(responseStr);
        return true;
    }
    else
    {
        qDebug("You have already logout");
        return false;
    }
}

bool DBConnect::login(const QString &dfserver, const QString &dfuser, const QString &dfpass)
{
    int port = 4433;
    int wait = 30; //seconds
    int dur = 3;
    QString web_svr = "dfws.datafax.com";

    QByteArray data;
    isconnected = false;
    if (!req.reqAuthorize(web_svr, port, wait, dfserver, dfuser, dfpass, dur, data)) {
                if (data.size() == 0) {
                    qDebug("Warning: no response from DFws service\n");
                    return false;
                } else {
                    responseStr = data;
                    emit failure(responseStr);
                    isconnected = true;
                    return true;
                }
    }
    else
    {
            responseStr = data;
            emit connected(responseStr);
            isconnected = true;
            return true;
    }
}
bool DBConnect::getKeys(int site){
    QByteArray data;
    if(!req.reqKeysBySite(site,data)){
//        qDebug() <<"fail to req keys by site: " << data;
        return false;
    }
    responseStr = data;
    return true;
}

bool DBConnect::getVisitsMap(){
    QByteArray data;
    if (req.reqVisitMap(data))
    {
        //qDebug() <<"get sites list in object" << data;
        responseStr = data;
        emit visitsMap(responseStr);
        return true;
    }
    else
    {
        qDebug("get visit failed");
        return false;
    }
}

void DBConnect::mergeKeyVisitsData(int site, QString pid){
    QString strReply;
    QJsonDocument jsonResponse;
    //QJsonObject jsonObject = jsonResponse.object();
    QJsonArray jsonKeysArray, jsonVisitsArray, jsonSetupArray;
    QMap<QString,QString> vMap;
    QMap<QString,QString> platesMap;


    //>>>>>>>>>>> get setup <<<<<<<<<<<<<<
    QJsonParseError error;
    this->getSetup();
    strReply = responseStr;

    jsonResponse = QJsonDocument::fromJson(strReply.toUtf8(),&error); // parsing
    qDebug() << error.errorString() ;
    jsonSetupArray= jsonResponse.object()["plates"].toArray(); // to array
    foreach (const QJsonValue & value, jsonSetupArray) {
        QJsonObject obj = value.toObject();
        //qDebug() << obj.toVariantHash()["description"] << " " << obj.toVariantHash()["number"];
        platesMap[QString::number(obj.toVariantHash()["number"].toDouble())] = obj.toVariantHash()["description"].toString();
    }

    //>>>>>>>>>>> get keys <<<<<<<<<<<<<<<
    this->getKeys(site);

    // store data from server
    strReply = responseStr;
    jsonResponse = QJsonDocument::fromJson(strReply.toUtf8()); // parsing
    jsonKeysArray= jsonResponse.array(); // to array

    //qDebug() << jsonKeysArray[0].toObject().toVariantHash();
//    foreach (const QJsonValue & value, jsonKeysArray) {
//        QJsonObject obj = value.toObject();
//        qDebug() << obj.toVariantHash();
//    }

    this->getVisitsMap();
    strReply = responseStr;
//    qDebug() << strReply;
    jsonResponse = QJsonDocument::fromJson(strReply.toUtf8()); // parsing
    jsonVisitsArray= jsonResponse.array(); // to array
    foreach (const QJsonValue & value, jsonVisitsArray) {
        QJsonObject obj = value.toObject();
        //qDebug() << obj.toVariantHash()["visitId"];

        //create a nice map
        //Debug() << QString::number(obj.toVariantHash()["visitId"].toDouble());
        vMap[QString::number(obj.toVariantHash()["visitId"].toDouble())] = QString(obj.toVariantHash()["label"].toString());
    }


    // Create a page-specific json structure
    //QJsonObject recordObject;
    QJsonArray visitIdArray;
    QJsonArray validvisitArray;

    QJsonObject recordObject;
    foreach (const QJsonValue & value, jsonVisitsArray) {
        QJsonObject obj = value.toObject();
        QJsonObject visitIdTable;
        visitIdTable.insert("vId", QString::number(obj.toVariantHash()["visitId"].toDouble()));
        visitIdTable.insert("vName", QString(obj.toVariantHash()["label"].toString()));
        visitIdTable.insert("requiredPlates", obj["requiredPlates"]);

        QJsonArray platNameMap;
        QVariantList::iterator myit;
        QVariantList mobj = obj.value("requiredPlates").toArray().toVariantList();
        for (myit = mobj.begin(); myit != mobj.end(); myit++){
            //qDebug() << myit->toString();
            if(platesMap.contains(myit->toString()))
                platNameMap.push_back(platesMap[myit->toString()]);
        }

        visitIdTable.insert("requiredPlatName", platNameMap);
        //qDebug() << obj.toVariantHash()["requiredPlates"];
        visitIdArray.push_back(visitIdTable);
    }


    // Key in use
    foreach (const QJsonValue & value, jsonKeysArray) {
        QJsonObject obj = value.toObject();
        // pid find.
        if((QString::number(obj.toVariantHash()["pid"].toDouble())) == pid){
            // create visit table here
            {
                QJsonObject validVisits;
                validVisits.insert("vId", QString::number(obj.toVariantHash()["visit"].toDouble()));
                validVisits.insert("plateId", vMap[QString::number(obj.toVariantHash()["plate"].toDouble())]);
                validvisitArray.push_back(validVisits);
            }
        }
    }

    //create "required plate table" for every visit and create "setup plate table" for the study

    recordObject.insert("pid",pid);
    recordObject.insert("visitTable", visitIdArray);
    recordObject.insert("validVisit", validvisitArray);


    // publish to singleton
//    qDebug()<< qml_singleton_interface->m_patientVisitMap;
}

bool DBConnect::getSetup(){
    QByteArray data;
    if(!req.reqSetup(data)){
        qDebug() <<"fail to req keys by site: " << data;
        responseStr = "";
        return false;
    }
    responseStr = data;
    // set to myJstr
    //qDebug() <<"set myJstr" << responseStr;
    jparse.setJsonStr(responseStr);

    return true;

}
