import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Page {
    //property var sObj: dCenter.sitesObjs;
   id:studyPage
   signal switchTo(int index)
   property var sitesObjs: {}
    header:
        Text
        {
            text:"DFexploreDB Server: "
        }
    Connections {
        target: DFexploreDB
        onStudiesList: showSites(result)
    }
    Component.onCompleted: {
        DFexploreDB.getStudies()
        console.log("Completed getStudies!")
    }

    // The model:
    ListModel {
        id: sitesModel
    }

    // The view:
    GridView {
        id: listView
        height: parent.height
        width: 300
        cellWidth: 120; cellHeight: 120
        anchors {
            left: parent.left; top: parent.top;
            right: parent.right; bottom: parent.bottom;
            margins: 20
        }
        model: sitesModel
        delegate: Item {
            width: 100; height: 100
            //text: title
            Image {
                id:myIcon
                y:10
                source: "images/Wiki.png"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text{text: "<i>"+name+"</i>"  +" ⋮ "+ "<b>" + id+ "</b> "
                wrapMode: Text.WordWrap
                width: parent.width
                anchors { top: myIcon.bottom; horizontalCenter: parent.horizontalCenter }
            }
            /*onClicked: {
                //message("clicked: "+name +":"+id);
                DFexploreDB.setStudyNumber(id)
                DFexploreDB.getSitesInfoList()
                studyPage.switchTo(1)
            }*/
            MouseArea {
                anchors.fill: parent
                onClicked: parent.GridView.view.currentIndex = index
            }
        }
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true

    }


    function showSites(msg) {

        console.log("studies: Studies Server Response:", msg)
        //sitesObjs = JSON.parse(msg);
        var sObj = JSON.parse(msg);
        var i=0;
        while (sObj[i])
        {
            //studiesPage.studiesDataModel.append(sObj[i]);
           // console.log("name: ",sObj[i].name,"id:", sObj[i].studyId)
            sitesModel.append({"name":sObj[i].name,"id": sObj[i].studyId})
            i++;
        }
        return "OK"
    }
}
