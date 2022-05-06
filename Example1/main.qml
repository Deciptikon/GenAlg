import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        id: childrenRect
        width: 200
        height: 300

        anchors.centerIn: parent

        color: "#0000ff"
    }

    Button {
        id: btTest
        text: "test"

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        property bool far: false

        onClicked: {
            if(btTest.far) {
                childrenRect.color = "#ff0000"
            } else {
                childrenRect.color = "#00ff00"
            }
            btTest.far = !btTest.far
        }
    }
}
