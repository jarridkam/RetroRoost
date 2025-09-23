import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "RetroRoost"

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: HomePage {}
    }
}
