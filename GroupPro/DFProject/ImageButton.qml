import QtQuick 2.0

Rectangle {
    width: parent.width
    height: 80
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

        }
    }

}

