import QtQuick
import QtQuick.Controls

Page {
    title: "Login"

    Column {
        spacing: 12
        anchors.centerIn: parent

        TextField {
            id: emailField
            placeholderText: "Email"
        }

        TextField {
            id: passwordField
            placeholderText: "Password"
            echoMode: TextInput.Password
        }

        Button {
            text: "Login"
            onClicked: {
                if (authorizationManager.login(emailField.text, passwordField.text)) {
                    stack.replace("HomePage.qml")

                } else {
                    console.log("Invalid email or password")
                }
            }
        }

    }

    Text {
        text: "Create an account"
        color: "blue"
        font.pixelSize: 12
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 10
        font.underline: true

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                stack.replace("CreateAccount.qml")
            }
        }
    }


}
