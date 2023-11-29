#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QtSql/QSqlDatabase>

class DatabaseConnection
{
public:
    DatabaseConnection(const QString &server,
                       const QString &driver,
                       const QString &user,
                       const QString &password,
                       const QString &databaseName);
    bool openDatabase(QString *error = nullptr);
    QSqlDatabase mDatabase;

private:
    QString mServer;
    QString mDriver;
    QString mUser;
    QString mPassword;
    QString mDatabaseName;
};

#endif // DATABASECONNECTION_H
