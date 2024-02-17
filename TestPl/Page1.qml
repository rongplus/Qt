import QtQuick 2.7
import test 1.0
import QtQuick.Dialogs 1.2
import org.example.io 1.0
Page1Form {
    A
    {

    }

    FileDialog {
        id: openDialog
        onAccepted: {

            io1.source = openDialog.fileUrl
            io1.read()
            console.log("file: j" + io1.text);
           textField1.text = io1.text
        }
    }

    FileIO
    {
        id:io1
        //source:"qmldir"

    }


    button1.onClicked: {
        console.log("Button Pressed. Entered text: " + textField1.text);

        openDialog.open();


    }
}
