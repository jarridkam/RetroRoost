#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "src/DatabaseManager.h"
#include "src/User/UserRepository.h"
#include <iostream>

bool isTest = false;

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Initialize user database
    if (!DatabaseManager::initUser()) {
        return -1;
    } else {
        std::cout << "userdb opened successfully" << std::endl;
    }

    // Initialize media database
    if (!DatabaseManager::initMedia()) {
        return -1;
    } else {
        std::cout << "mediadb opened successfully" << std::endl;
    }

    if (!UserRepository::createUser("Alice", "alice@example.com")) {
        qWarning() << "Insert failed!";
    }


    QList<User> users = UserRepository::getAllUsers();
    for (const auto &u : users) {
        qDebug() << "User:" << u.GetName()
                 << "Password:" << u.GetPassword();
    }


    // Load the correct QML file
    if (!isTest) {
        engine.load(QUrl(QStringLiteral("qrc:/QML/main.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;
    } else {
        engine.load(QUrl(QStringLiteral("qrc:/QML/test.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;
    }

    return app.exec();
}
