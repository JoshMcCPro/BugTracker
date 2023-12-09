#ifndef USERDETAILSWIDGET_H
#define USERDETAILSWIDGET_H

#include <QWidget>
#include "user.h"

namespace Ui {
class UserDetailsWidget;
}

class UserDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserDetailsWidget(User* user, QWidget *parent = nullptr);
    ~UserDetailsWidget();

    void setUserDetails();

signals:
    void openProjects();

private slots:
    void on_pb_Dashboard_clicked();

    void on_pb_Projects_clicked();

    void on_pb_Messages_clicked();

    void on_pb_Settings_clicked();

private:
    Ui::UserDetailsWidget *ui;

    User* user;
};

#endif // USERDETAILSWIDGET_H
