import QtQuick 2.0

Item {
    id:root
    Rectangle {
           id: sky
           width: parent.width
           height: 200
           gradient: Gradient {
               GradientStop { position: 0.0; color: "#0080FF" }
               GradientStop { position: 1.0; color: "#66CCFF" }
           }
       }
       Rectangle {
           id: ground
           anchors.top: sky.bottom
           anchors.bottom: parent.bottom
           width: parent.width
           gradient: Gradient {
               GradientStop { position: 0.0; color: "#00FF00" }
               GradientStop { position: 1.0; color: "#00803F" }
           }
       }

    ClickableImageV2 {
           id: greenBox
           x: 40; y: parent.height-height
           source: "assets/vs.png"
           text: "animation on property"
           NumberAnimation on y {
               to: 40; duration: 4000
           }
       }

    ClickableImageV2 {
           id: blueBox
           x: (parent.width-width)/2; y: parent.height-height
           source: "assets/safari.png"
           text: "behavior on property"
           Behavior on y {
               NumberAnimation { duration: 4000 }
           }

           onClicked: y = 40
           // random y on each click
   //        onClicked: y = 40+Math.random()*(205-40)
       }


    ClickableImageV2 {
            id: redBox
            x: parent.width-width-40; y: parent.height-height
            source: "assets/robot.png"
            onClicked: anim.start()
    //        onClicked: anim.restart()

            text: "standalone animation"

            NumberAnimation {
                id: anim
                target: redBox
                properties: "y"
                to: 40
                duration: 4000
            }
        }


    Image {
            id: ball
            x: 0; y: parent.height-height
            source: "assets/background.png"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    ball.x = 0;
                    ball.y = parent.height-ball.height;
                    ball.rotation = 0;
                    anim123.restart()
                    anim456.restart()
                }
            }


        }
    SequentialAnimation {
        id: anim123
        NumberAnimation {
            target: ball
            properties: "y"
            to: 120
            duration: parent.duration * 0.4
        }
        NumberAnimation {
            target: ball
            properties: "x"
            to: 240
            duration: parent.duration * 0.6
        }
        RotationAnimation {
                target: ball
                properties: "rotation"
                to: 720
                duration: parent.duration
            }
    }


    ParallelAnimation {
            id: anim456
            SequentialAnimation {
                NumberAnimation {
                    target: ball
                    properties: "y"
                    to: 20
                    duration: root.duration * 0.4
                    easing.type: Easing.OutCirc
                }
                NumberAnimation {
                    target: ball
                    properties: "y"
                    to: root.height-ball.height
                    duration: root.duration * 0.6
                    easing.type: Easing.OutBounce
                }
            }
            NumberAnimation {
                target: ball
                properties: "x"
                to: root.width-ball.width
                duration: root.duration
            }
            RotationAnimation {
                target: ball
                properties: "rotation"
                to: 720
                duration: root.duration
            }
        }
}
