#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "databaseconnection.h"
#include "userauthdialog.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

    // verify user login credentials
    bool isEmailValid(QString enteredEmail);
    bool isPasswordValid(QString enteredPassword);
    bool isFormatValid(QString format, QString userCredential);
    bool doesEmailExist(QString enteredEmail);
    bool isPasswordCorrect();

    // get database info
    QString getSaltFromDatabase(QString email);
    QString getEncryptedPasswordFromDatabase(QString email);

    // encrypt password
    QString encryptPassword(const QString &password, const QString &salt, int iterations, quint64 keyLength);
    bool bDoesPasswordExist(const QString &enteredPassword, const QString &storedPassword, const QString &salt, int iterations, quint64 keylength);

    QWidget *mainParentWidget;
    UserAuthDialog *userAuthDialog;

signals:
    void loginSuccessful(QString email);

private slots:
    void on_pb_login_clicked();

    void on_pb_signup_clicked();

    void on_le_email_editingFinished();

    void on_le_password_editingFinished();

private:
    Ui::LoginWidget *ui;

    // get pages

    // SQL DB
    QString mDbConnectionName = "loginConnection";
    DatabaseConnection mDbConnection;

    // user login credential formats
    QString emailFormat = "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}$";
    QString passwordFormat = "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)[A-Za-z0-9]+$";
};

#endif // LOGINWIDGET_H
