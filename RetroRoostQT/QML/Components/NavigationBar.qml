import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    height: 50
    color: "#333"

    Row {
        anchors.centerIn: parent
        spacing: 20

        Button { text: "Home" }
        Button { text: "Profile" }
        Button { text: "Settings" }
    }
}
