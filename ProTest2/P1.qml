import QtQuick 2.0

Rectangle {

    //anchors.fill: parent
    x:0
    y:0
    width: 200
    height: 200
    Column {
            id: row
            anchors.left: parent.left
            spacing: 8
            R1 { }
            G1 { width: 96 }
            B1 { }
        }


    Row {
            id: rrr1
            anchors.bottom: parent.bottom
            spacing: 8
            R1 { }
            G1 { }
            B1 { }
        }

    Grid {
            id: grid
            rows: 2
            anchors.centerIn: parent
            spacing: 8
            R1 { }
            G1 { }
            B1 { }
            R1 { }
            G1 { }
            B1 { }
            R1 { }
            G1 { }
            B1 { }
        }


}
