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
                       const QString &databaseName,
                       const QString &connectionName);

    bool openDatabase(QString *error = nullptr);
    void closeDatabase();
    void insertValues(QString mDbConnectionName, QString tableName, QMap<QString, QVariant>);

private:
    QSqlDatabase mDatabase;
    QString mServer;
    QString mDriver;
    QString mUser;
    QString mPassword;
    QString mDatabaseName;
};

#endif // DATABASECONNECTION_H
