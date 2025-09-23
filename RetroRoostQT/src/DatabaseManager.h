// DatabaseManager.h
#pragma once
#include <QSqlDatabase>

class DatabaseManager {
public:
    static bool initUser();
    static bool initMedia();
    static QSqlDatabase connection(const QString &which = "user");

private:
    static QSqlDatabase userdb;
    static QSqlDatabase mediadb;
};
