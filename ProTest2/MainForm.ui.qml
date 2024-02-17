import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Item {
    width: 640
    height: 480

    property alias button1: button1
    property alias button2: button2

    RowLayout {
        anchors.verticalCenterOffset: 153
        anchors.horizontalCenterOffset: -25
        anchors.centerIn: parent

        Button {
            id: button1
            text: qsTr("Press Me 1")
        }

        Button {
            id: button2
            text: qsTr("Press Me 2")
        }
    }

    Image {
        id: image
        x: 175
        y: 40
        width: 345
        height: 207
        source: "https://www.google.ca/images/branding/googlelogo/2x/googlelogo_color_120x44dp.png"
    }
}
