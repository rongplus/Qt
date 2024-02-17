import QtQuick 2.2
import QtQuick.Controls 1.3
import QtQuick.Window 2.0

Rectangle {

    anchors.fill: parent
    Text{
        text: qsTr("Good morning")
    }

    Button
    {
        text: qsTr("Clicke me")
    }
    Button
    {
        text: qsTr("Close")
        onClicked:
        {
            //Qt.quit()
            close()
        }
    }

}
