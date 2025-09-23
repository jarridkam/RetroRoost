import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Retro Roost"

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: SplashScreen {}
    }
}
