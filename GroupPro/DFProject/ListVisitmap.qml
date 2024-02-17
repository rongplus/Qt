import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Page {
    //property var sObj: dCenter.sitesObjs;
   //property var sitesObjs: {}
    header:
        Text
        {
            text:"Site id> Subjects"
        }
    Connections {
        target: DFexploreDB
        onVisitsMap: showSites(result)
    }
    Component.onCompleted: {
        DFexploreDB.setStudyNumber(154)
        console.log("getVisitsMap Running!")
        DFexploreDB.getVisitsMap()

        console.log("Completed getVisitsMap!")
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
                source: "images/linux.png"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text{
                text: "<i>"+centerId+"</i>"  +" ⋮ "+ "<b>" + affiliation+ "</b> "
                wrapMode: Text.WordWrap
                width: parent.width
                anchors { top: myIcon.bottom; horizontalCenter: parent.horizontalCenter }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: parent.GridView.view.currentIndex = index
            }

        }
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true

    }


    function showSites(msg) {

        console.log("visit map: Studies Server Response:", msg)
        var sitesObjs = JSON.parse(msg);
        sitesModel.clear()
        var i=0;
        while (sitesObjs[i])
        {
            //console.log("list visitmap + " + sitesObjs[i]);
             //sitesModel.append({"PID":pid1, "Status": sta, "imgsource": statusIcon(sta)});
            sitesModel.append({"centerId":sitesObjs[i].label, "affiliation": sitesObjs[i].dueDay});
            i++;
        }
        return "OK"
    }
}
