#include "userdetailswidget.h"
#include "ui_userdetailswidget.h"
#include "user.h"

UserDetailsWidget::UserDetailsWidget(User* user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserDetailsWidget),
    user(user)
{
    ui->setupUi(this);

    setUserDetails();
}

UserDetailsWidget::~UserDetailsWidget()
{
    delete ui;
}

// sets the user info in widget, picture, name, etc
void UserDetailsWidget::setUserDetails()
{
    if (!user) {
        qDebug() << "Userdetails ERROR: User is Invalid";
        return;
    }

    QString username = (user->getFirstName() + " " + user->getLastName());
    ui->lb_username->setText(username);

}

// emits signal to open dashboard
void UserDetailsWidget::on_pb_Dashboard_clicked()
{

}

// emits signal to open projects page
void UserDetailsWidget::on_pb_Projects_clicked()
{
    // insert project page into main window
    emit openProjects();
}

// emits signal to open messages page
void UserDetailsWidget::on_pb_Messages_clicked()
{

}

// emits signal to open settings page
void UserDetailsWidget::on_pb_Settings_clicked()
{

}

