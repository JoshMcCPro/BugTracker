#include "signupwidget.h"
#include "ui_signupwidget.h"
#include "userauthdialog.h"
#include "databaseconnection.h"
#include "verifycodewidget.h"
#include "userregistration.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QRandomGenerator>
#include <QtEndian>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QCryptographicHash>
#include <QByteArray>
#include <QPasswordDigestor>

SignUpWidget::SignUpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUpWidget),
    mDbConnection("serverbugtracker.database.windows.net",
                  "SQL SERVER",
                  "JoshMcCPro",
                  "Jam8122233302!",
                  "DB_BUGTRACKER",
                  mDbConnectionName)
{
    ui->setupUi(this);
}

SignUpWidget::~SignUpWidget()
{
    delete ui;
    delete mainParentWidget;
    delete userAuthDialog;
}


void SignUpWidget::on_pb_signup_clicked()
{
    QString enteredEmail = ui->le_email->text();
    QString enteredFirstName = ui->le_firstName->text();
    QString enteredLastName = ui->le_lastName->text();
    QString enteredPassword = ui->le_password->text();
    QString enteredConfirmPassword = ui->le_confirmPassword->text();

    // check if email is valid
    if (!isEmailValid(enteredEmail)) {
        return;
    }

    // check if first name is valid
    if (!isFirstNameValid(enteredFirstName)) {
        return;
    }

    // check if last name is valid
    if(!isLastNameValid(enteredLastName)) {
        return;
    }

    // check if password is valid
    if(!isPasswordValid(enteredPassword)) {
        return;
    }

    // check if confirm password is valid
    if(!(enteredConfirmPassword == enteredPassword)) {
        return;
    }

    // connect to db
    QString error;
    if(!mDbConnection.openDatabase(&error)) {
        qDebug() << error;
        return;
    }

    qDebug() << "Connected to DB";

    // check if this email already exists
    // if an email does exist stop
    if(doesEmailExist(enteredEmail)) {
        qDebug() << "Email already exists";
        mDbConnection.closeDatabase();
        return;
    }

    // encrypt password
    const QString saltHex = generateSaltHex();
    const int iterations = 12;
    const quint64 keyLength = 20;

    const QString encryptedPwdHex = encryptPasswword(enteredPassword, saltHex, iterations, keyLength);

    // store user registration info
    UserRegistration user(enteredEmail, enteredFirstName, enteredLastName, encryptedPwdHex, saltHex);

    qDebug() << "Storing User Registration ...";
    user.displayUserInfo();

    // close db connection
    mDbConnection.closeDatabase();

    // sends verify code
    // go to verification code page
    goToVerifyCodePage(user);

}


void SignUpWidget::on_pb_login_clicked()
{
    mainParentWidget = this->parentWidget()->parentWidget();

    // Use qobject_cast for type-safe casting
    if (userAuthDialog = qobject_cast<UserAuthDialog*>(mainParentWidget)) {
        qDebug() << "Login Page!";
        userAuthDialog->getLoginPage();
    } else {
        // Handle the case where the cast is not successful
        qDebug() << "Error: Parent is not of type UserAuthDialog";
        qDebug() << "Parent: " << mainParentWidget->metaObject()->className();
    }
}


void SignUpWidget::on_le_email_editingFinished()
{
    QString enteredEmail = ui->le_email->text();
    // check if email is valid
    if (!isEmailValid(enteredEmail)) {
        return;
    }
}


void SignUpWidget::on_le_firstName_editingFinished()
{
    QString enteredFirstName = ui->le_firstName->text();
    // check if first name is valid
    if (!isFirstNameValid(enteredFirstName)) {
        return;
    }
}


void SignUpWidget::on_le_lastName_editingFinished()
{
    QString enteredLastName = ui->le_lastName->text();
    // check if last name is valid
    if (!isLastNameValid(enteredLastName)) {
        return;
    }
}


void SignUpWidget::on_le_password_editingFinished()
{
    QString enteredPassword = ui->le_password->text();
    // check if password is valid
    if(!isPasswordValid(enteredPassword)) {
        return;
    }
}


void SignUpWidget::on_le_confirmPassword_editingFinished()
{
    QString enteredPassword = ui->le_password->text();
    QString enteredConfirmPassword = ui->le_confirmPassword->text();
    // check if confirm password is valid
    if(!(enteredConfirmPassword == enteredPassword)) {
        return;
    }
}


bool SignUpWidget::isEmailValid(QString enteredEmail)
{
    QString email = enteredEmail;

    if (email.isEmpty()) {
        qDebug() << "INVALID: email is empty!";
        return false;
    }

    // check if email format is valid
    if (!isFormatValid(emailFormat, email)) {
        qDebug() << "INVALID: email format error";
        return false;
    }

    // Check if email meets the length requirements
    if (!(email.length() <= 100)) {
        qDebug() << "INVALID: email to large!";
        return false;
    }

    return true;
}


bool SignUpWidget::isFirstNameValid(QString enteredFirstName)
{
    QString firstName = enteredFirstName;

    if(firstName.isEmpty()) {
        qDebug() << "INVALID: first name is empty!";
        return false;
    }

    if(!(firstName.length() <= 50)) {
        qDebug() << "INVALID: first name needs to be at most 50 characters!";
        return false;
    }

    return true;
}

bool SignUpWidget::isLastNameValid(QString enteredLastName)
{
    QString lastName = enteredLastName;

    if(lastName.isEmpty()) {
        qDebug() << "INVALID: first name is empty!";
        return false;
    }

    if(!(lastName.length() <= 50)) {
        qDebug() << "INVALID: first name needs to be at most 50 characters!";
        return false;
    }

    return true;
}

bool SignUpWidget::isPasswordValid(QString enteredPassword)
{
    QString password = enteredPassword;

    if (password.isEmpty()) {
        qDebug() << "INVALID: password is empty!";
        return false;
    }

    // check if password format is valid
    if(!isFormatValid(passwordFormat, password)) {
        qDebug() << "INVALID: password needs at least 1 captial, 1 lower, and 1 number";
        return false;
    }

    // check if password meets the length requirments
    if (!(password.length() >= 8)) {
        qDebug() << "INVALID: password needs to be at least 8 characters!";
        return false;
    }

    return true;
}

bool SignUpWidget::isFormatValid(QString format, QString userCredential)
{
    // make format
    QRegularExpression regEx(format);

    // email format validator
    QRegularExpressionValidator regExValidator(regEx);

    // check if email format is valid
    int pos = 0;
    if (regExValidator.validate(userCredential, pos) == QValidator::Acceptable) {
        return true;
    }

    return false;
}

bool SignUpWidget::doesEmailExist(QString enteredEmail)
{
    QSqlQuery query(QSqlDatabase::database(mDbConnectionName));

    query.prepare("SELECT COUNT(*) FROM users WHERE email = :email");
    query.bindValue(":email", enteredEmail);

    if (!query.exec()) {
        qDebug() << "ERROR executing query: " << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return (count > 0);
    }

    qDebug() << "Unexpected error: Query result not available.";
    return false;
}

QString SignUpWidget::generateSaltHex()
{
    QRandomGenerator randomGenerator = QRandomGenerator::securelySeeded();
    QByteArray salt;

    for (int i = 0; i < 2; ++i) {
        quint32 randomNum = randomGenerator.global()->generate();

        randomNum = qToLittleEndian(randomNum);

        salt.append(reinterpret_cast<const char*>(&randomNum), sizeof(randomNum));
    }

    salt.resize(8);

    return salt.toHex();
}

QString SignUpWidget::encryptPasswword(const QString &password, const QString &salt, int iterations, quint64 keylength)
{
    // Convert the password and salt to UTF-8 encoding
    QByteArray passwordData = password.toUtf8();
    QByteArray saltData = QByteArray::fromHex(salt.toUtf8());

    // Perform PBKDF2 key derivation
    QByteArray derivedKey = QPasswordDigestor::deriveKeyPbkdf1(QCryptographicHash::Sha1, passwordData, saltData, iterations, keylength);
    // Convert the derived key to a hex-encoding string
    QString derivedKeyHex = QString(derivedKey.toHex());

    return derivedKeyHex;
}

void SignUpWidget::goToVerifyCodePage(const UserRegistration& user)
{
    mainParentWidget = this->parentWidget()->parentWidget();

    // Use qobject_cast for type-safe casting
    if (userAuthDialog = qobject_cast<UserAuthDialog*>(mainParentWidget)) {
        qDebug() << "Verify Code Page!";
        userAuthDialog->getVerifyCodePage(user);
    } else {
        // Handle the case where the cast is not successful
        qDebug() << "Error: Parent is not of type UserAuthDialog";
        qDebug() << "Parent: " << mainParentWidget->metaObject()->className();
    }

}

