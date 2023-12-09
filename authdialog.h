#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include "databaseconnection.h"

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog();
    // verify if email exists
    bool doesEmailExist(QString email);

    void storeUserData();
    void sendUserData();

    /*USER LOGIN*/

    // verify password
    bool bDoesPasswordExist(const QString &enteredPassword, const QString &storedPassword, const QString &salt, int iterations, quint64 keylength);

    /*USER SIGNUP*/

    // Verify Sign Up

    // verify email format
    bool verifyEmailFormat();
    // verify first name format
    bool verifyFirstNameFormat();
    // verify last name format
    bool verifyLastNameFormat();
    // verify password format
    bool verifyPasswordFormat();
    // verify confirm password
    bool verifyConfirmPassword();

    // Encrypt Sign Up

    // encrypt salt
    QString generateSalt();
    // encrypt password
    QString encryptPassword(const QString &password, const QString &salt, int iterations, quint64 keylength);
    // generate email verification code
    QString generateVerificationCode(int codeLength);
    // send email verification code
    void sendEmailVerificationCode(QString recieverEmail);

private slots:
    /*USER LOGIN*/

    void on_pb_login_login_clicked();

    void on_pb_signuppage_login_clicked();

    /*USER SIGNUP*/

    void on_pb_loginpage_signup_clicked();

    void on_pb_signup_signup_clicked();

    void on_le_email_signup_editingFinished();

    void on_le_firstname_signup_editingFinished();

    void on_le_lastname_signup_editingFinished();

    void on_le_password_signup_editingFinished();

    void on_le_confirmpwd_signup_editingFinished();

private:
    Ui::AuthDialog *ui;
    // SQL Database
    DatabaseConnection mDbConnection;

    // User Sign Up
    QString emailSP;
    QString firstNameSP;
    QString lastNameSP;
    QString passwordSP;
    QString confirmPasswordSP;
    QString emailCode;

    // Pages
    int emailCodeIndex = 2;
    int signupIndex = 1;
    int loginIndex = 0;
};

#endif // AUTHDIALOG_H
