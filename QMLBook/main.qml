import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1 {
        }

        P1
        {

        }
        P2
        {

        }

        TrafficLight
        {

        }

        Db1
        {

        }
        Myweb
        {

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
        }

        TabButton {
            text: qsTr("ball")
        }

        TabButton {
            text: qsTr("traffic")
        }

        TabButton {
            text: qsTr("db")
        }
        TabButton {
            text: qsTr("web")
        }

    }
}
