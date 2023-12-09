#ifndef CREATEPROJECTWIDGET_H
#define CREATEPROJECTWIDGET_H

#include <QWidget>
#include "databaseconnection.h"

namespace Ui {
class CreateProjectWidget;
}

class CreateProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CreateProjectWidget(QWidget *parent = nullptr);
    ~CreateProjectWidget();

    void createProject();

signals:
    void ProjectCreated();

private slots:
    void on_pb_Create_clicked();

private:
    Ui::CreateProjectWidget *ui;

    // SQL DB
    QString mDbConnectionName = "createProjectConnection";
    DatabaseConnection mDbConnection;
};

#endif // CREATEPROJECTWIDGET_H
