#ifndef PROJECTMANAGERWIDGET_H
#define PROJECTMANAGERWIDGET_H

#include <QWidget>
#include <QStackedWidget>

#include "databaseconnection.h"
#include "ProjectManagement/newprojectwidget.h"
#include "ProjectManagement/createprojectwidget.h"
#include "ProjectManagement/joinprojectwidget.h"
#include "ProjectManagement/projectbutton.h"
#include "user.h"
#include "project.h"

namespace Ui {
class ProjectManagerWidget;
}

class ProjectManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectManagerWidget(const User& user, QWidget *parent = nullptr);
    ~ProjectManagerWidget();

    // handles opening all project manager pages
    void openPage(QString pageName);

    void openDefaultPage();

signals:
    void projectButtonClicked(const Project& project);

public slots:
    void handleOpenCreateProject();

    void handleOpenJoinProject();

private slots:
    void on_pb_NewProject_clicked();

    void handleOnProjectButtonClicked(const Project& project);

private:
    /*VARIABLES*/
    Ui::ProjectManagerWidget *ui;

    // user/owner
    const User mUser;

    QString mDbConnectionName = "projectManagerConnection";
    // sql server database
    DatabaseConnection mDbConnection;

    // project info page holder
    QStackedWidget *projectInfoStackedWidget;

    // new project page
    NewProjectWidget *newProjectWidget;

    // create project page
    CreateProjectWidget *createProjectWidget;

    // join project page
    JoinProjectWidget *joinProjectWidget;

    /*FUNCTIONS*/
    void createProjectButtons();

    QList<Project> getProjectsFromDatabase();

};

#endif // PROJECTMANAGERWIDGET_H
