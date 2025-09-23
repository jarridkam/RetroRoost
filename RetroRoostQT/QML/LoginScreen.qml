import QtQuick
import QtQuick.Controls

Page {
    title: "Login"

    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField {
            placeholderText: "Username"
        }
        TextField {
            placeholderText: "Password"
            echoMode: TextInput.Password
        }
        Button {
            text: "Login"
            onClicked: stack.replace("HomePage.qml")
        }
    }
}
