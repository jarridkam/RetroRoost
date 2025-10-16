import QtQuick
import QtQuick.Controls

Page {
    title: "Create Account"

    Column {
        anchors.centerIn: parent
        spacing: 10

        TextField {
            id: usernameField
            placeholderText: "Username"
        }

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
            text: "Create Account"
            onClicked: {
                if (userRepo.createUser(usernameField.text,
                    emailField.text,
                    passwordField.text)) {
                    stack.replace("HomePage.qml")
                } else {
                    console.log("Failed to create account")
                }
            }
        }

    }
}