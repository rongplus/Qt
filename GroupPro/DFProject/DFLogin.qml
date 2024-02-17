import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Page {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    id:root

    Image {
        id: logo
        source: "images/cdsi_logo.png"
        x:0
        y:0
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        Layout.fillWidth: true
    }

    Button {
        id: button
        x: 140
        y: 202
        text: qsTr("Cancel")
        onClicked: {
            //var a = Qt.createComponent()
            var newObject = Qt.createQmlObject('import QtQuick 2.0; Rectangle {color: "red"; width: 20; height: 20}',
                                                 root,
                                                 "dynamicSnippet1");
            var newObject1 = Qt.createQmlObject('import QtQuick.Controls 2.1; Button {  width: 120; height: 30; text:"New"; x:60 ;onClicked: {console.log("Button Pressed.  text: " +  button.text)}}',
                                                 root,
                                                 "dynamicSnippet2");
        }

    }

    Button {
        id: button1
        x: 10
        y: 202
        text: qsTr("Login")
        onClicked: {
            console.log("Button Pressed. Entered text: " + textField1.text);
             console.log("Button Pressed. MyDB text: " + MyDB.getName());
            root.StackView.view.push("qrc:/MainView.qml")
        }

    }



    Label {
        id: label
        x: 10
        y: 81
        text: qsTr("User name")
    }

    Label {
        id: label1
        x: 10
        y: 135
        text: qsTr("Password")
    }

    CheckBox {
        id: checkBox
        x: 320
        y: 135
        text: qsTr("Remenber me")
    }

    TextField {
        id: textField
        x: 110
        y: 69
        text: qsTr("Name")
    }

    TextField {
        id: textField1
        x: 110
        y: 128
        text: qsTr("Pasword")
    }

    Connections
    {
        target: MyDB
        onDataChanged:
        {
            text1.text= MyDB.getCurrentDateTime()
            console.log("The application data changed!")
        }
    }

    Text {
        id: text1
        x: 92
        y: 320
        text: qsTr("Text")
        wrapMode: Text.WordWrap
        font.pixelSize: 12
    }



}
