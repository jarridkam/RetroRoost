// UserDatabase.h
#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <QSqlDatabase>
#include <QString>
#include <QVariant>

class UserDatabase {
public:
    explicit UserDatabase(const QString& dbPath  = "users.db");
    ~UserDatabase();

    bool open();
    void close();

    static bool addUser(const QString& username, const QString& password);
    static bool userExists(const QString& username) ;
    static bool checkLogin(const QString& username, const QString& password) ;

private:
    static bool initTables();
    QSqlDatabase db;
    QString dbPath;
};

#endif // USERDATABASE_H
