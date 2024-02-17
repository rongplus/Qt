import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Text {

        text: "Qt Quick"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pointSize: 24
    }


    Row {

            Slider {

                id: slider
                from:1
                to:200
                value:100

            }

            Label {

                text: Math.floor(slider.value)
            }
        }

    Rectangle {
            x: 20
            y: 20
            width: 100; height: 100
            color: "forestgreen"

            NumberAnimation on x { to: 250; duration: 1000 }
            NumberAnimation on y { to: 250; duration: 1000 }
        }

    Text {
            id: helloText
            text: "Hello world!"
            x:200
            y: 30
            anchors.horizontalCenter: page.horizontalCenter
            font.pointSize: 24; font.bold: true

            MouseArea { id: mouseArea; anchors.fill: parent }

            states: State {
                name: "down"; when: mouseArea.pressed == true
                PropertyChanges { target: helloText; y: 160; rotation: 180; color: "red" }
            }

            transitions: Transition {
                from: ""; to: "down"; reversible: true
                ParallelAnimation {
                    NumberAnimation { properties: "y,rotation"; duration: 500; easing.type: Easing.InOutQuad }
                    ColorAnimation { duration: 500 }
                }
            }
        }


    Canvas {

            //anchors.fill: parent
        id: line_c
            x:0

            //y: parent.height- 400
            y: parent.height - 200
            width:400
            height: 200
            onPaint: {

                var ctx = getContext("2d");
                ctx.fillStyle = "lightslategray"

                ctx.beginPath();
                ctx.fillRect(10, 10, 80, 50);

                ctx.beginPath();
                ctx.fillRect(120, 10, 70, 70);

                ctx.beginPath();
                ctx.ellipse(230, 10, 90, 70);
                ctx.fill();

                ctx.beginPath();
                ctx.ellipse(10, 110, 70, 70);
                ctx.fill();

                ctx.beginPath();
                ctx.roundedRect(120, 110, 70, 70, 10, 10);
                ctx.fill();

                ctx.beginPath();
                ctx.moveTo(230, 110);
                ctx.arc(230, 110, 70, 0, Math.PI * 0.5, false);
                ctx.fill();

            }

    }

    Canvas {
        id: sstock_c
        width: 240; height: 120
        y:200
        x:200
        onPaint: {
            var ctx = getContext("2d")
            ctx.strokeStyle = "blue"
            ctx.lineWidth = 4

            ctx.beginPath()
            ctx.rect(20, 20, 60, 60)

            ctx.translate(50,-20)
            ctx.stroke()

            // draw path now rotated
            ctx.strokeStyle = "green"
            ctx.rotate(Math.PI/4)
            ctx.rect(20, 20, 60, 60)
            ctx.stroke()

        }
    }
/**/
    Canvas {
        id: shallow_c
        width: 120; height: 120
        onPaint: {
            var ctx = getContext("2d")
            // setup a blue shadow
                    ctx.shadowColor = "#2ed5fa";
                    ctx.shadowOffsetX = 2;
                    ctx.shadowOffsetY = 2;
                    ctx.shadowBlur = 10;

            // render green text
                    ctx.font = 'bold 80px Ubuntu';
                    ctx.fillStyle = "#24d12e";
                    ctx.fillText("Canvas!",30,180);
        }
    }

    Canvas {
        id: gradint_c
        width: 120; height: 120
        onPaint: {
            var ctx = getContext("2d")
            var gradient = ctx.createLinearGradient(100,0,100,200)
            gradient.addColorStop(0, "blue")
            gradient.addColorStop(0.5, "lightsteelblue")
            ctx.fillStyle = gradient
            ctx.fillRect(50,50,100,100)
        }
    }

    Canvas {
        id: root
        width: 120; height: 120
        onPaint: {
            var ctx = getContext("2d")
            ctx.fillStyle = 'green'
            ctx.strokeStyle = "blue"
            ctx.lineWidth = 4

            // draw a filles rectangle
            ctx.fillRect(20, 20, 80, 80)
            // cut our an inner rectangle
            ctx.clearRect(30,30, 60, 60)
            // stroke a border from top-left to
            // inner center of the larger rectangle
            ctx.strokeRect(20,20, 40, 40)
        }
    }

    Q2
    {
        id: q2_2
        anchors.top: root.bottom
    }

    /*

    Row {
            id: colorTools
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 8
            }
            property variant activeSquare: red
            property color paintColor: "#33B5E5"
            spacing: 4
            Repeater {
                model: ["#33B5E5", "#99CC00", "#FFBB33", "#FF4444"]
                ColorSquare {
                    id: red
                    color: modelData
                    active: parent.paintColor == color
                    onClicked: {
                        parent.paintColor = color
                    }
                }
            }
        }
    Canvas {
            id: canvas
            anchors {
                left: parent.left
                right: parent.right
                top: colorTools.bottom
                bottom: parent.bottom
                margins: 8
            }
            property real lastX
            property real lastY
            property color color: colorTools.paintColor

            onPaint: {
                var ctx = getContext('2d')
                ctx.lineWidth = 1.5
                ctx.strokeStyle = canvas.color
                ctx.beginPath()
                ctx.moveTo(lastX, lastY)
                lastX = area.mouseX
                lastY = area.mouseY
                ctx.lineTo(lastX, lastY)
                ctx.stroke()
            }
            MouseArea {
                id: area
                anchors.fill: parent
                onPressed: {
                    canvas.lastX = mouseX
                    canvas.lastY = mouseY
                }
                onPositionChanged: {
                    canvas.requestPaint()
                }
            }
        }
        */
}
