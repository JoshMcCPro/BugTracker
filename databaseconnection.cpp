#include "databaseconnection.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

DatabaseConnection::DatabaseConnection(const QString &server,
                                       const QString &driver,
                                       const QString &user,
                                       const QString &password,
                                       const QString &databaseName)
{
    mDatabase = QSqlDatabase::addDatabase("QODBC");
    mDatabaseName = databaseName;
    mServer = server;
    mDriver = driver;
    mUser = user;
    mPassword = password;
}

bool DatabaseConnection::openDatabase(QString *error)
{
    mDatabase.setDatabaseName(QString("Driver={%1};Server=%2;"
                                      "Database=%3;Uid=%4;"
                                      "Pwd=%5;")
                                  .arg(mDriver)
                                  .arg(mServer)
                                  .arg(mDatabaseName)
                                  .arg(mUser)
                                  .arg(mPassword));
    if(!mDatabase.open()) {
        if (error != nullptr) {
            *error = mDatabase.lastError().text();
        }
        return false;
    }
    return true;
}
