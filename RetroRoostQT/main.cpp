#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "C++/User/UserDatabase.h"

bool isTest = false;

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    UserDatabase user_database;
    if (!user_database.open()) {
        return -1; // Exit if DB fails
    }

    if (!isTest) {
        engine.load(QUrl(QStringLiteral("qrc:/QML/main.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;
    }
    else {
        engine.load(QUrl(QStringLiteral("qrc:/QML/test.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;
    }


    return app.exec();


}
