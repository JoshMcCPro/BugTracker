#include "applicationmanager.h"
#include "mainwindow.h"
#include "databaseconnection.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

ApplicationManager::ApplicationManager(QObject *parent)
    : QObject{parent},
    mDbConnection("serverbugtracker.database.windows.net",
                  "SQL SERVER",
                  "JoshMcCPro",
                  "Jam8122233302!",
                  "DB_BUGTRACKER",
                  mDbConnectionName),
    // initializing pointers
    user(nullptr),
    mainWindow(nullptr)
{

}

ApplicationManager::~ApplicationManager()
{
    delete mainWindow;
    delete user;
}

// get user info from sql server database
User *ApplicationManager::getUserInfo(const QString &email)
{
    // connect to db
    QString error;
    if(!mDbConnection.openDatabase(&error)) {
        qDebug() << "Falied to open database: " << error;
        mDbConnection.closeDatabase();
        return nullptr;
    }

    User* user = nullptr;
    QSqlQuery query(QSqlDatabase::database(mDbConnectionName));

    if (!query.prepare("SELECT * FROM users WHERE email = :email")) {
        qDebug() << "AppManager ERROR: preparing query";
        mDbConnection.closeDatabase();
        return nullptr;
    }
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {

        // get values from columns
        int userId = query.value("user_id").toInt();
        QString userEmail = query.value("email").toString();
        QString userFirstName = query.value("first_name").toString();
        QString userLastName = query.value("last_name").toString();
        QDateTime userRegistrationDate = query.value("registration_date").toDateTime();

        qDebug() << "id: " << userId;
        qDebug() << "email: " << userEmail;
        qDebug() << "first name: " << userFirstName;
        qDebug() << "last name: " << userLastName;
        qDebug() << "registration date: " << userRegistrationDate;

        user = new User(userId, userEmail, userFirstName, userLastName, userRegistrationDate);

    } else {
        if (query.lastError().type() != QSqlError::NoError) {
            qDebug() << "Query execution error: " << query.lastError().text();
        } else {
            qDebug() << "User not found from email: " << email;
        }
    }

    mDbConnection.closeDatabase();
    return user;
}

void ApplicationManager::handleLoginSuccess(QString email)
{
    // get user info using the user email
    user = getUserInfo(email);

    if(!user) {
        qDebug() << "AppManager ERROR: failed to retrieve user info";
        return;
    }

    mainWindow = new MainWindow(user);
    mainWindow->show();
}
