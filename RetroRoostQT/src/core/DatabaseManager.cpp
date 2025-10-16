#include "DatabaseManager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>


QSqlDatabase DatabaseManager::s_userDb;
QSqlDatabase DatabaseManager::s_mediaDb;

static QString resolveAppDataDirectory()
{
    QString base = qEnvironmentVariable("RETRO_ROOST_DATA_DIR");
    if (base.isEmpty()) {
        if (QCoreApplication::organizationName().isEmpty())
            QCoreApplication::setOrganizationName("RetroRoost");
        if (QCoreApplication::applicationName().isEmpty())
            QCoreApplication::setApplicationName("RetroRoostQT");

        base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (base.isEmpty())
            base = QDir::homePath() + "/RetroRoost";
    }

    QDir dir(base);
    if (!dir.exists())
        dir.mkpath(".");

    return dir.absolutePath();
}


static QSqlDatabase connectPostgres(const QString& name, const QString& dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", name);
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName(dbName);
    db.setUserName("retro_roost_user");
    db.setPassword(qEnvironmentVariable("POSTGRES_PASSWORD"));

    if (!db.open()) {
        qWarning() << "❌ Connection failed for" << dbName << ":" << db.lastError().text();
    } else {
        qDebug() << "✅ Connected to database:" << dbName;
    }

    return db;
}



static bool ensureUsersTableAndIndexes(QSqlDatabase& db)
{
    QSqlQuery query(db);

    // 1) Create users table if missing
    const char* createSql =
        "CREATE TABLE IF NOT EXISTS users ("
        "  id SERIAL PRIMARY KEY,"
        "  name TEXT NOT NULL,"
        "  email TEXT NOT NULL,"
        "  password TEXT DEFAULT '' NOT NULL"
        ")";
    if (!query.exec(createSql)) {
        qWarning() << "[users] create failed:" << query.lastError().text();
        return false;
    }

    // 2) Create unique indexes
    if (!query.exec("CREATE UNIQUE INDEX IF NOT EXISTS idx_users_name ON users(name)")) {
        qWarning() << "[users] unique index on name failed:" << query.lastError().text();
        return false;
    }

    if (!query.exec("CREATE UNIQUE INDEX IF NOT EXISTS idx_users_email ON users(email)")) {
        qWarning() << "[users] unique index on email failed:" << query.lastError().text();
        return false;
    }

    qDebug() << "[users] schema ready (PostgreSQL)";
    return true;
}

//API

bool DatabaseManager::initUserDatabase()
{
    s_userDb = connectPostgres("user", "users");
    if (!s_userDb.isOpen())
        return false;

    if (!ensureUsersTableAndIndexes(s_userDb))
        return false;

    return true;
}

bool DatabaseManager::initMediaDatabase()
{
    s_mediaDb = connectPostgres("media", "media");
    if (!s_mediaDb.isOpen())
        return false;

    QSqlQuery createMediaTable(s_mediaDb);
    const char* createSql =
        "CREATE TABLE IF NOT EXISTS media ("
        "  id SERIAL PRIMARY KEY,"
        "  title TEXT NOT NULL"
        ")";
    if (!createMediaTable.exec(createSql)) {
        qWarning() << "Failed to create media table:" << createMediaTable.lastError().text();
        return false;
    }

    qDebug() << "[media] table ready (PostgreSQL)";
    return true;
}

QSqlDatabase DatabaseManager::connection(const QString& which)
{
    if (which.compare(QStringLiteral("media"), Qt::CaseInsensitive) == 0)
        return s_mediaDb;
    return s_userDb;
}
