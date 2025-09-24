#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>

// static members must be defined once
QSqlDatabase DatabaseManager::userdb;
QSqlDatabase DatabaseManager::mediadb;

QString appDataDir()
{
    // Optional override for debugging or custom installs
    QString base = qEnvironmentVariable("RETRO_ROOST_DATA_DIR");
    if (base.isEmpty()) {
        if (QCoreApplication::organizationName().isEmpty())
            QCoreApplication::setOrganizationName("RetroRoost");
        if (QCoreApplication::applicationName().isEmpty())
            QCoreApplication::setApplicationName("RetroRoostQT");

        // Windows:  %LOCALAPPDATA%/RetroRoost/RetroRoostQT
        // macOS:    ~/Library/Application Support/RetroRoost/RetroRoostQT
        // Linux:    ~/.local/share/RetroRoost/RetroRoostQT
        base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (base.isEmpty())
            base = QDir::homePath() + "/RetroRoost";
    }

    QDir dir(base);
    if (!dir.exists())
        dir.mkpath(".");
    return dir.absolutePath();
}

QString appDataFile(const QString& fileName)
{
    QDir dir(appDataDir());
    return dir.filePath(fileName);
}

static bool ensureUsersSchema(QSqlDatabase& db)
{
    QSqlQuery q(db);

    // 1) Original create (id, name, email) — safe if table exists already
    if (!q.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  name TEXT NOT NULL, "
        "  email TEXT NOT NULL"
        ")"))
    {
        qWarning() << "[users] create failed:" << q.lastError().text();
        return false;
    }

    // 2) MIGRATION: add password column if missing
    QSqlQuery iq(db);
    if (!iq.exec("PRAGMA table_info(users)")) {
        qWarning() << "[users] PRAGMA table_info failed:" << iq.lastError().text();
        return false;
    }

    bool hasPassword = false;
    while (iq.next()) {
        // PRAGMA table_info columns: cid, name, type, notnull, dflt_value, pk
        if (iq.value(1).toString() == QLatin1String("password")) { hasPassword = true; break; }
    }

    if (!hasPassword) {
        QSqlQuery alter(db);
        if (!alter.exec("ALTER TABLE users ADD COLUMN password TEXT NOT NULL DEFAULT ''")) {
            qWarning() << "[users] add password column failed:" << alter.lastError().text();
            return false;
        }
        qDebug() << "[users] Added password column";
    }

    // 3) UNIQUE indexes (safe if they already exist)
    if (!q.exec("CREATE UNIQUE INDEX IF NOT EXISTS idx_users_name  ON users(name)")) {
        qWarning() << "[users] unique idx on name failed:" << q.lastError().text();
        return false;
    }
    if (!q.exec("CREATE UNIQUE INDEX IF NOT EXISTS idx_users_email ON users(email)")) {
        qWarning() << "[users] unique idx on email failed:" << q.lastError().text();
        return false;
    }

    qDebug() << "[users] schema ready";
    return true;
}

bool DatabaseManager::initUser() {
    userdb = QSqlDatabase::addDatabase("QSQLITE", "user");
    const QString userPath = appDataFile("userdb.sqlite");
    userdb.setDatabaseName(userPath);

    if (!userdb.open()) {
        qWarning() << "Failed to open user DB:" << userdb.lastError().text()
                   << "at" << userPath;
        return false;
    }
    qDebug() << "userdb opened at:" << userdb.databaseName();

    if (!ensureUsersSchema(userdb))
        return false;

    return true;
}

bool DatabaseManager::initMedia() {
    mediadb = QSqlDatabase::addDatabase("QSQLITE", "media");
    const QString mediaPath = appDataFile("mediadb.sqlite");
    mediadb.setDatabaseName(mediaPath);

    if (!mediadb.open()) {
        qWarning() << "Failed to open media DB:" << mediadb.lastError().text()
                   << "at" << mediaPath;
        return false;
    }
    qDebug() << "mediadb opened at:" << mediadb.databaseName();

    QSqlQuery q(mediadb);
    if (!q.exec(
        "CREATE TABLE IF NOT EXISTS media ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  title TEXT NOT NULL"
        ")"))
    {
        qWarning() << "Failed to create media table:" << q.lastError().text();
        return false;
    }
    qDebug() << "media table ready";
    return true;
}

QSqlDatabase DatabaseManager::connection(const QString &which) {
    if (which == "media") return mediadb;
    return userdb;
}
