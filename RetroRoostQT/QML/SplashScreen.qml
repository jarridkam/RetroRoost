import QtQuick
import QtQuick.Controls

Page {
    title: "Splash"

    Rectangle {
        anchors.fill: parent
        color: "red"

        Text {
            anchors.centerIn: parent
            text: "RetroRoost"
            color: "white"
            font.pixelSize: 48
            font.bold: true
        }
    }

    Timer {
        interval: 2000        // 2 seconds
        running: true
        repeat: false
        onTriggered: stack.replace("LoginScreen.qml")
    }
}
