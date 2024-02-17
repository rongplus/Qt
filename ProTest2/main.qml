import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.0
import Qt.labs.platform 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

     MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("close")
                onTriggered: Qt.quit();
            }
            MenuItem {
                text: qsTr("Edit")
                onTriggered: Qt.quit();
            }
        }
    }

     ToolBar {
         RowLayout {
                       anchors.fill: parent
                       ToolButton {
                           text: qsTr("‹")
                           onClicked: stack.pop()
                       }
                       Label {
                           text: "Title"
                           elide: Label.ElideRight
                           horizontalAlignment: Qt.AlignHCenter
                           verticalAlignment: Qt.AlignVCenter
                           Layout.fillWidth: true
                       }
                       ToolButton {
                           text: qsTr("⋮")
                           onClicked: menu.open()
                       }
                   }
     }

     background: Rectangle {
               gradient: Gradient {
                   GradientStop { position: 0; color: "#ffffff" }
                   GradientStop { position: 1; color: "#c1bbf9" }
               }
           }

     header: TabBar {
                 id:o1
                 TabButton {
                     text: qsTr("Second")
                     onClicked:  t2.text = p1.button1.text

                 }
                 TabButton {
                     text: qsTr("Third")
                 }
                 TabButton {
                     text: qsTr("Login")
                 }
     }

    SwipeView {
            id: swipeView
            anchors.fill: parent
            currentIndex: tabBar.currentIndex


            MainForm {
               // anchors.fill: parent
                button1.onClicked: messageDialog.show(qsTr("Button 1 pressed"))
                button2.onClicked: messageDialog.show(qsTr("Button 2 pressed"))
            }


            P1
            {

            }


            P2
            {

            }
            P3{

            }

            Login
            {

            }

    }







    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }


    footer: TabBar {
            id: tabBar
            currentIndex: swipeView.currentIndex
            TabButton {
                text: qsTr("First")

            }
            TabButton {
                text: qsTr("Second")
                onClicked:  t2.text = p1.button1.text

            }
            TabButton {
                text: qsTr("Third")
            }
            TabButton {
                text: qsTr("list")
            }
            TabButton {
                text: qsTr("Login")
            }
        }

}
