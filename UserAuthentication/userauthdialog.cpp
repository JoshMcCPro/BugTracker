#include "userauthdialog.h"
#include "ui_userauthdialog.h"
#include "loginwidget.h"
#include "signupwidget.h"
#include "verifycodewidget.h"
#include "userregistration.h"

#include <QStackedWidget>

UserAuthDialog::UserAuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserAuthDialog)
{
    ui->setupUi(this);

    stackedWidget = ui->stackedWidget;
    loginWidget = new LoginWidget(this);
    signUpWidget = new SignUpWidget(this);
    verifyCodeWidget = new VerifyCodeWidget(this);

    QObject::connect(loginWidget, &LoginWidget::loginSuccessful, this, &UserAuthDialog::handleLoginSuccessful);

    stackedWidget->addWidget(loginWidget);
    stackedWidget->addWidget(signUpWidget);
    stackedWidget->addWidget(verifyCodeWidget);

    stackedWidget->setCurrentIndex(loginIndex);

}

UserAuthDialog::~UserAuthDialog()
{
    delete ui;
    delete stackedWidget;
}

void UserAuthDialog::setPage(int pageIndex)
{
    stackedWidget->setCurrentIndex(pageIndex);
}

void UserAuthDialog::getLoginPage()
{
    stackedWidget->setCurrentIndex(loginIndex);
}

void UserAuthDialog::getSignUpPage()
{
    stackedWidget->setCurrentIndex(signUpIndex);
}

void UserAuthDialog::getVerifyCodePage(const UserRegistration& user)
{
    stackedWidget->setCurrentIndex(verifyCodeIndex);
    verifyCodeWidget->setUserInformation(user);
    verifyCodeWidget->sendEmailVerificationCode();
}

void UserAuthDialog::handleLoginSuccessful(QString email)
{
    // close the dialog
    accept();

    // Emit the loginSuccessful signal
    emit loginSuccessful(email);
}
