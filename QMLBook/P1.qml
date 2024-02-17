import QtQuick 2.5
import "fisrtJS.js" as CCount
Image {
    id: root
    source: "assets/background.png"

    property int padding: 40
    property int duration: 400
    property bool running: false

    Image {
        id: box
        x: root.padding;
        y: (root.height-height)/2
        source: "assets/box_green.png"

        NumberAnimation on x {
            to: root.width - box.width - root.padding
            duration: root.duration
            running: root.running
        }
        RotationAnimation on rotation {
            to: 360
            duration: root.duration
            running: root.running
        }
    }

    FF1 {
        id:aaa
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.running = true
            aaa.runJS()
            CCount.countDown()
        }
    }

}
