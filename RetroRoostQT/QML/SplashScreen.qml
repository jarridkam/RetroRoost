import QtQuick
import QtQuick.Controls

Page {
    title: "Splash"

    Rectangle {
        anchors.fill: parent
        color: "black"

        Text {
            anchors.centerIn: parent
            text: "Loading RetroRoost..."
            color: "white"
            font.pixelSize: 24
        }
    }

    Timer {
        interval: 2000        // 2 seconds
        running: true
        repeat: false
        onTriggered: stack.replace("LoginScreen.qml")
    }
}
