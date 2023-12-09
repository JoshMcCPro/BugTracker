#ifndef NEWPROJECTWIDGET_H
#define NEWPROJECTWIDGET_H

#include <QWidget>

namespace Ui {
class NewProjectWidget;
}

class NewProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NewProjectWidget(QWidget *parent = nullptr);
    ~NewProjectWidget();

signals:
    void openCreateProject();

    void openJoinProject();

private slots:

    void on_pb_CreateProject_clicked();

    void on_pb_JoinProject_clicked();

private:
    Ui::NewProjectWidget *ui;
};

#endif // NEWPROJECTWIDGET_H
