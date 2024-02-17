import QtQuick 2.4
import QtQuick.Controls 2.1

Item {
    width: 400
    height: 400

    Button {
        id: button
        x: 88
        y: 282
        text: qsTr("Login")
    }

    Button {
        id: button1
        x: 213
        y: 282
        text: qsTr("Cancel")
    }

    Image {
        id: image
        x: 71
        y: 21
        width: 235
        height: 70
        source: "https://www.google.ca/images/branding/googlelogo/2x/googlelogo_color_120x44dp.png"
    }

    TextInput {
        id: textInput
        x: 196
        y: 127
        width: 80
        height: 20
        text: qsTr("Text Input")
        font.pixelSize: 12
    }

    Text {
        id: text1
        x: 83
        y: 127
        text: qsTr("Name")
        styleColor: "#d72828"
        font.pixelSize: 12
    }

    Text {
        id: text2
        x: 83
        y: 193
        text: qsTr("Password")
        styleColor: "#bf1b1b"
        font.pixelSize: 12
    }

    TextInput {
        id: textInput1
        x: 181
        y: 188
        width: 80
        height: 20
        text: qsTr("Text Input")
        font.pixelSize: 12
    }

    Rectangle {
        id: rectangle
        x: 71
        y: 165
        width: 200
        height: 55
        color: "#ffffff"
        z: -1
    }

    Image {
        id: image1
        x: 16
        y: 214
        width: 100
        height: 100
        source: "images/a2.jpg"
    }
}
