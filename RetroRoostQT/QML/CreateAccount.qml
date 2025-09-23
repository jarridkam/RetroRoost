import QtQuick
import QtQuick.Controls

Page {
    title: "Create Account"

    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField {
            placeholderText: "Username"
        }

        TextField {
            placeholderText: "Email"
        }

        TextField {
            placeholderText: "Password"
            echoMode: TextInput.Password
        }
        Button {
            text: "Create Account"

            onClicked: {

                stack.replace("HomePage.qml")

            }
        }
    }
}
