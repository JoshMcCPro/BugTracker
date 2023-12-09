#include "verifycodewidget.h"
#include "ui_verifycodewidget.h"
#include "userregistration.h"
#include "databaseconnection.h"

#include <QRandomGenerator>
#include <QSslSocket>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>

VerifyCodeWidget::VerifyCodeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerifyCodeWidget),
    mDbConnection("serverbugtracker.database.windows.net",
                  "SQL SERVER",
                  "JoshMcCPro",
                  "Jam8122233302!",
                  "DB_BUGTRACKER",
                  mDbConnectionName)
{
    ui->setupUi(this);
}

VerifyCodeWidget::~VerifyCodeWidget()
{
    delete ui;
}

void VerifyCodeWidget::setUserInformation(const UserRegistration &user)
{
    this->user = user;
}

void VerifyCodeWidget::displayUserInfo()
{
    user.displayUserInfo();
}

QString VerifyCodeWidget::generateVerificationCode()
{
    qDebug() << "Generating Code ...";
    QRandomGenerator randCodeGen = QRandomGenerator::securelySeeded();
    QVector<int> verificationCode(verificationCodeLength, 0);
    QString Code = "";

    for (int i = 0; i < verificationCodeLength; i++) {
        quint32 randNum = randCodeGen.bounded(0,9);
        verificationCode[i] = randNum;
        Code += QString::number(randNum);
    }

    qDebug() << "Code: " << Code;
    return Code;
}

void VerifyCodeWidget::sendEmailVerificationCode()
{
    qDebug() << "Sending email ...";
    QString recieverEmail = user.email;
    emailCode = generateVerificationCode();

    // Construct the email message
    QString subject = "Email Verification Code";
    QString body = "Your verification code is: " + emailCode;
    QString from = senderEmail;
    QString to = recieverEmail;

    // Connect to the SMTP server
    QSslSocket socket;
    socket.setProtocol(QSsl::TlsV1_3);

    socket.connectToHostEncrypted(host, port);
    if (!socket.waitForConnected()) {
        qDebug() << "Error connecting to the SMTP server: " << socket.errorString();
        return;
    }

    // Read the welcome message from the server
    if (!socket.waitForReadyRead()) {
        qDebug() << "Error reading welcome message: " << socket.errorString();
        return;
    }
    qDebug() << "Server says: " << socket.readAll();

    // Send the HELO command
    socket.write("HELO localhost\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send the login command
    socket.write("AUTH LOGIN\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send the username (Gmail address)
    socket.write((senderEmail.toUtf8() + "\r\n").toBase64() + "\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send app password
    socket.write((appPassword.toUtf8() + "\r\n").toBase64() + "\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send the MAIL FROM command
    socket.write(("MAIL FROM: <" + from + ">\r\n").toUtf8());
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send the RCPT TO command
    socket.write(("RCPT TO: <" + to + ">\r\n").toUtf8());
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says:" << socket.readAll();

    // Send the DATA command
    socket.write("DATA\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says:" << socket.readAll();

    // Construct and send the email message
    QString message =
        "From: " + from + "\r\n"
        "To: " + to + "\r\n"
        "Subject: " + subject + "\r\n"
        "\r\n" + body + "\r\n"
        ".\r\n";  // End of message marker

    socket.write(message.toUtf8());
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says:" << socket.readAll();

    // Send the QUIT command
    socket.write("QUIT\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says:" << socket.readAll();

    // Close the socket
    socket.close();
    qDebug() << "Email sent";
}

void VerifyCodeWidget::confirmUserAccount()
{
    QString error;
    if(!mDbConnection.openDatabase(&error)) {
        qDebug() << "Database connection error: " <<error;
        return;
    }

    QSqlQuery query(QSqlDatabase::database(mDbConnectionName));

    if (!query.prepare("INSERT INTO users (email, first_name, last_name)"
                      "VALUES (:email, :first_name, :last_name)")) {
        qDebug() << "Error preparing query: " << query.lastError().text();
        QSqlDatabase::database(mDbConnectionName).rollback();
        return;
    }

    query.bindValue(":email", user.email);
    query.bindValue(":first_name", user.firstName);
    query.bindValue(":last_name", user.lastName);

    if (!query.exec()) {
        qDebug() << "Error executing user query: " << query.lastError().text();
        QSqlDatabase::database(mDbConnectionName).rollback();
        return;
    }

    // Get the user ID of the newly inserted user
    int userId = query.lastInsertId().toInt();

    qDebug() << "User info sent to database. User ID: " << userId;

    // Now insert the password into the "passwords" table using the obtained user ID
    if (!query.prepare("INSERT INTO passwords (user_id, password_hash, salt) "
                       "VALUES (:user_id, :password_hash, :salt)")) {
        qDebug() << "Error preparing password query: " << query.lastError().text();
        QSqlDatabase::database(mDbConnectionName).rollback();
        return;
    }

    query.bindValue(":user_id", userId);
    query.bindValue(":password_hash", user.password);
    query.bindValue(":salt", user.salt);

    if (!query.exec()) {
        qDebug() << "Error executing password query: " << query.lastError().text();
        QSqlDatabase::database(mDbConnectionName).rollback();
        return;
    }

    qDebug() << "Password info sent to database";

    // Commit the transaction
    QSqlDatabase::database(mDbConnectionName).commit();

    mDbConnection.closeDatabase();
}

void VerifyCodeWidget::goToLoginPage()
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

void VerifyCodeWidget::on_pb_submitCode_clicked()
{
    QString enteredVerificationCode = ui->le_verificationCode->text();
    if(enteredVerificationCode == emailCode) {
        confirmUserAccount();
        goToLoginPage();
    }
    else {
        qDebug() << "code: false";
    }
}

