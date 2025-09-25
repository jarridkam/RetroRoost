#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "src/AuthorizationManager.h"
#include "src/DatabaseManager.h"
#include "src/User/UserRepository.h"
#include <iostream>

bool isTest = false;

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    UserRepository userRepo;
    AuthorizationManager authorizationManager;

    if (!DatabaseManager::initUserDatabase()) {
        return -1;
    } else {
        std::cout << "userdb opened successfully" << std::endl;
    }


    if (!DatabaseManager::initMediaDatabase()) {
        return -1;
    } else {
        std::cout << "mediadb opened successfully" << std::endl;
    }

    engine.rootContext()->setContextProperty("userRepo", &userRepo);
    engine.rootContext()->setContextProperty("authorizationManager", &authorizationManager);


    /*if (!UserRepository::createUser("BlakeSaul", "blakey@example.com", "testPassword")) {
        qWarning() << "Insert failed!";
    }*/


    QList<User> users = userRepo.getAllUsers();
    for (const auto &u : users) {
        qDebug() << "User:" << u.GetName()
                 << "Email" << u.GetEmail()
                 << "Password:" << u.GetPassword();
    }


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
