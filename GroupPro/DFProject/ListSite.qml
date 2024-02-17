import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4

Page {
    id: page
    //property var sObj: dCenter.sitesObjs;
   //property var sitesObjs: {}
    header:
        Text
        {
            text:"Study ID > Sites "
        }
    Connections {
        target: DFexploreDB
        onSitesInfoList: showSites(result)
    }
    Component.onCompleted: {
        DFexploreDB.setStudyNumber(154)
        DFexploreDB.getSitesInfoList()

        console.log("Completed getSitesInfoList!")
    }

    // The model:
    ListModel {
        id: sitesModel
    }

    // The view:
   /* ListView {
        id: listView
        height: 300
        anchors.rightMargin: 40
        anchors.bottomMargin: -32
        anchors.leftMargin: 0
        anchors.topMargin: 72
        width: 80
        anchors {
            left: parent.left; top: parent.top;
            right: parent.right;
            margins: 20
        }
        model: sitesModel
        delegate: ItemDelegate {
            //text: title
            text: "<i>"+centerId+"</i>"  +" ⋮ "+ "<b>" + affiliation+ "</b> "  +"<br> <small> n participant </small>"
            onClicked: {
                message("clicked: "+centerId +":"+affiliation);
            }
        }
    }
*/
    ListModel
    {
        id: gridModel
    }

    GridView {
        width: parent.width;
        height: 200
        cellWidth: 120; cellHeight: 120
        //anchors.fill: parent
        model: gridModel

        delegate: Item {
            width: 100; height: 100
            Image { id: myIcon
                 y:10
                source: portrait;
                anchors.horizontalCenter: parent.horizontalCenter }
            Text { text: name;
                 wrapMode: Text.WordWrap
                anchors { top: myIcon.bottom; horizontalCenter: parent.horizontalCenter } }
            MouseArea {
                anchors.fill: parent
                onClicked: parent.GridView.view.currentIndex = index
            }
        }

        highlight: Rectangle { width: 80; height: 80; color: "lightsteelblue" }
        focus: true

    }

    Rectangle
    {
        x:0
        y:205
        width: parent.width
        height: 200
       // color: "lightsteelblue"

         Image {
             id: image
             x: 10
             y: 10;
             width:parent.width
             height:300
             scale: 1
             source: "images/FusionChartGraph.jpg"
         }
    }



    function showSites(msg) {

        console.log("studies: Studies Server Response:", msg)
        var sitesObjs = JSON.parse(msg);
        sitesModel.clear()
        gridModel.clear()
        var i=0;
        while (sitesObjs[i])
        {
            sitesModel.append({"centerId":sitesObjs[i].centerId, "affiliation": sitesObjs[i].affiliation});
            gridModel.append({"name":sitesObjs[i].affiliation, "portrait":"images/site.jpeg"});
            i++;
        }
        return "OK"
    }
}
