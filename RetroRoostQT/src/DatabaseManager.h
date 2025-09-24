#pragma once
#include <QString>
#include <QSqlDatabase>

class DatabaseManager {
public:
    // Opens (and migrates) the user + media databases
    static bool initUserDatabase();
    static bool initMediaDatabase();

    // Returns the requested connection ("user" or "media")
    static QSqlDatabase connection(const QString& which);

private:
    // Static connection handles (initialized in .cpp)
    static QSqlDatabase s_userDb;
    static QSqlDatabase s_mediaDb;
};
