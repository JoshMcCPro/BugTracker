#include "databaseconnection.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

DatabaseConnection::DatabaseConnection(const QString &server,
                                       const QString &driver,
                                       const QString &user,
                                       const QString &password,
                                       const QString &databaseName,
                                       const QString &connectionName)
    : mDatabase(QSqlDatabase::addDatabase("QODBC", connectionName)),
    mDatabaseName(databaseName),
    mServer(server),
    mDriver(driver),
    mUser(user),
    mPassword(password)
{

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

    qDebug() << "Connection Details: " << mDatabase.connectionName();
    qDebug() << "Driver: " << mDriver;
    qDebug() << "Server: " << mServer;
    qDebug() << "Database: " << mDatabaseName;
    qDebug() << "User: " << mUser;
    qDebug() << "Password: " << mPassword;

    if(!mDatabase.open()) {
        if (error != nullptr) {
            *error = mDatabase.lastError().text();
        }
        return false;
    }
    return true;
}

void DatabaseConnection::closeDatabase()
{
    mDatabase.close();
}

void DatabaseConnection::insertValues(QString mDbConnectionName, QString tableName, QMap<QString, QVariant> columnValues)
{

    QSqlQuery query(QSqlDatabase::database(mDbConnectionName));

    QStringList columns;
    QStringList placeholders;

    // Create the column names and placeholders
    for (const QString& column : columnValues.keys()) {
        columns << column;
        placeholders << ":" + column;
    }

    // Build the query string
    QString queryString = QString("INSERT INTO %1 (%2) VALUES (%3)")
                              .arg(tableName)
                              .arg(columns.join(", "))
                              .arg(placeholders.join(", "));

    if (!query.prepare(queryString)) {
        qDebug() << "Error preparing query: " << query.lastError().text();
        QSqlDatabase::database(mDbConnectionName).rollback();
        return;
    }

    // Bind values to placeholders
    for (const QString& column : columnValues.keys()) {
        query.bindValue(":" + column, columnValues.value(column));
    }

    if (!query.exec()) {
        qDebug() << "Error executing user query: " << query.lastError().text();
        QSqlDatabase::database(mDbConnectionName).rollback();
        return;
    }

    // Get the user ID of the newly inserted user
    int id = query.lastInsertId().toInt();

    qDebug() << "info sent to database. ID: " << id;

    // Commit the transaction
    QSqlDatabase::database(mDbConnectionName).commit();

}
