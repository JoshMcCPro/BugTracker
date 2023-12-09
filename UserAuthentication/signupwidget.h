#ifndef SIGNUPWIDGET_H
#define SIGNUPWIDGET_H

#include <QWidget>
#include "databaseconnection.h"
#include "userauthdialog.h"

class UserRegistration;

namespace Ui {
class SignUpWidget;
}

class SignUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SignUpWidget(QWidget *parent = nullptr);
    ~SignUpWidget();

    // verify user sign up credentials
    bool isEmailValid(QString enteredEmail);
    bool isFirstNameValid(QString enteredFirstName);
    bool isLastNameValid(QString enteredLastName);
    bool isPasswordValid(QString enteredPassword);
    bool isFormatValid(QString format, QString userCredential);
    bool doesEmailExist(QString enteredEmail);

    // Encrypt Sign Up

    // password encryption
    QString generateSaltHex();
    QString encryptPasswword(const QString& password, const QString &salt, int iterations, quint64 keylength);

    // pages
    void goToVerifyCodePage(const UserRegistration& user);

    QWidget *mainParentWidget;
    UserAuthDialog *userAuthDialog;


private slots:
    void on_pb_signup_clicked();

    void on_pb_login_clicked();

    void on_le_email_editingFinished();

    void on_le_firstName_editingFinished();

    void on_le_lastName_editingFinished();

    void on_le_password_editingFinished();

    void on_le_confirmPassword_editingFinished();

private:
    Ui::SignUpWidget *ui;

    // SQL DB
    QString mDbConnectionName = "signUpConnection";
    DatabaseConnection mDbConnection;

    // user sign up credential formats
    QString emailFormat = "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}$";
    QString passwordFormat = "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)[A-Za-z0-9]+$";
};

#endif // SIGNUPWIDGET_H
