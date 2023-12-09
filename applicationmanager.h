#ifndef APPLICATIONMANAGER_H
#define APPLICATIONMANAGER_H

#include <QObject>
#include "mainwindow.h"
#include "databaseconnection.h"
#include "user.h"

class ApplicationManager : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationManager(QObject *parent = nullptr);
    ~ApplicationManager();

    User* getUserInfo(const QString& email);

public slots:
    // gets the emitted login Success from userAuthLogin class
    void handleLoginSuccess(QString email);

signals:
    void requestLogout();

private:

    // Sql database
    QString mDbConnectionName = "appManagerConnection";
    DatabaseConnection mDbConnection;

    // User
    User* user;

    // Main Window
    MainWindow *mainWindow;
};

#endif // APPLICATIONMANAGER_H
