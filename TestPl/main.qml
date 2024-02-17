import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import test 1.0
import QtQuick.Dialogs 1.2
import org.example.io 1.0

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

        Page {
            Label
            {
                text: "io1.text"
                anchors.centerIn: parent
            }


            Rectangle {
              width: 360
              height: 360
              Text {
                id: textField
                text: qsTr("Click me.")
                anchors.centerIn: parent
              }
              MouseArea {
                anchors.fill: parent
                onClicked: {
                  textField.text = "Clicked...";

                  // Passing objects from QML to C++
                  b.doA(aa);
                  b.doC(c);

                  // Setting a property works as expected as well.
                  aa.test = "foobar";
                  b.doA(aa);
                  b.doC(c);

                  // Dynamic object creation in QML
                  var newA = Qt.createQmlObject('import QtQuick 2.7; import test 1.0; A {test: "foo"}', parent);
                  b.doA(newA);

                  // Object creation in C++ and passing to QML
                  var anotherA = b.makeA();
                  console.log(anotherA.test);

                  // Use an object created in C++ in QML.
                  c.a = anotherA;
                  b.doC(c);

                  // Using QML lists
                  console.log("");
                  d.doVl();
                  console.log("");
                  b.doVl(d.vl);

                  // It is also possible to pass dynamically created lists.
                  console.log("");
                  b.doVl(["j1", 2, 3, "j4"]);

                  // See the code of b.h to see how to access the objects in the list from C++.
                  console.log("");
                  b.doAlist([aa, aaa]);

                  // Create a list of objects in C++ and use it in QML.
                  console.log("");
                  d.vl = b.makeAList();
                  console.log(d.vl[0].test);
                  console.log(d.vl[1].test);
                }
              }
              A{id: aa; test: "bar"}
                A{id: aaa; test: "blah"}
                B{id: b}
                C{id: c; a: aa}
                D{id: d;  vl: ["i1", "i2", 3, 4, aa]}
            }
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
    }
}
