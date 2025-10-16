#pragma once

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    static bool initUserDatabase();
    static bool initMediaDatabase();
    static QSqlDatabase connection(const QString& which);

private:
    // Static database handles for the app's lifetime
    static QSqlDatabase s_userDb;
    static QSqlDatabase s_mediaDb;

    // Prevent direct construction or copying
    DatabaseManager() = delete;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
};
