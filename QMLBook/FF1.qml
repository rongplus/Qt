import QtQuick 2.0

Item {

    MessageBox1
    {
        id:m_ok
    }

    function runJS() {
        console.log("Your JS code goes here");
        m_ok.visible = true;
      }

}
