import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2

Page {

    property int naccountme:  swipeView.count
    Connections {
        target: p1
        onSwitchTo: switchTo(index)
    }

    visible: true
        width: parent.width
        height: parent.height
        title: qsTr("Hello World")
        id:root
        SwipeView {
            id: swipeView
            anchors.fill: parent
           // currentIndex: tabBar.currentIndex

            ListStudy {
                id:p1
            }

            ListSite {
                id:p2

            }

            ListSubject
            {
                id:p3
            }
            ListVisitmap
            {
                id:p4
            }


        }

        /*footer: TabBar {
            id: tabBar
            currentIndex: swipeView.currentIndex
            TabButton {id:tb1
                text: qsTr("Study")
            }
            TabButton {
                text: qsTr("Site")

            }
            TabButton {
                text: qsTr("Subject")
            }
            TabButton {
                text: qsTr("Visit")
            }

        }*/

        footer:Row {
            spacing: 2

            Rectangle {  width: parent.width/naccountme; height: 80
                Text {
                    id: btnStudy
                    width: parent.width
                    text: qsTr("Study")
                    horizontalAlignment:Text.AlignHCenter
                }
                Image {

                    source: "images/Wiki.png"
                    width: 50
                    height: 50
                    y:30
                    x:parent.width/2 - 25
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton

                    onClicked: swipeView.currentIndex = 0
                }
            }
            Rectangle {  width: parent.width/naccountme; height: 80
                Text {
                    id: btnSite
                    width: parent.width
                    text: qsTr("Site")
                    horizontalAlignment:Text.AlignHCenter
                }
                Image {

                    source: "images/site.jpeg"
                    width: 50
                    height: 50
                    y:30
                    x:parent.width/2 - 25
                }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton

                    onClicked: swipeView.currentIndex = 1
                }
            }
            Rectangle {  width: parent.width/naccountme; height: 80
                Text {
                    id: btnSubject
                    width: parent.width
                    text: qsTr("Subject")
                    horizontalAlignment:Text.AlignHCenter
                }
                Image {

                    source: "images/yyy.png"
                    width: 50
                    height: 50
                    y:30
                    x:parent.width/2 - 25
                }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton

                    onClicked: swipeView.currentIndex = 2
                }
            }
            Rectangle {  width: parent.width/naccountme; height: 80
                Text {
                    id: btnPlate
                    width: parent.width
                    text: qsTr("Subject")
                    horizontalAlignment:Text.AlignHCenter
                }
                Image {
                    enabled: true
                    source: "images/linux.png"
                    width: 50
                    height: 50
                    y:30
                    x:parent.width/2 - 25

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton

                        onClicked: swipeView.currentIndex = 3
                    }
                }

            }

        }

        function switchTo( index)
        {
            tabBar.currentIndex = index
        }
}
