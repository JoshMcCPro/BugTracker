#include "projectmanagerwidget.h"
#include "ui_projectmanagerwidget.h"
#include "ProjectManagement/newprojectwidget.h"
#include "ProjectManagement/createprojectwidget.h"
#include "ProjectManagement/joinprojectwidget.h"
#include "ProjectManagement/projectbutton.h"
#include "project.h"

#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QVariant>

ProjectManagerWidget::ProjectManagerWidget(const User& user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectManagerWidget),
    mUser(user),
    mDbConnection("serverbugtracker.database.windows.net",
                  "SQL SERVER",
                  "JoshMcCPro",
                  "Jam8122233302!",
                  "DB_BUGTRACKER",
                  mDbConnectionName)
{
    ui->setupUi(this);

    projectInfoStackedWidget = ui->projectInfoStackedWidget;
    createProjectButtons();

    // initialize widgets
    newProjectWidget = nullptr;
    createProjectWidget = nullptr;
    joinProjectWidget = nullptr;

}

ProjectManagerWidget::~ProjectManagerWidget()
{
    delete ui;
    delete projectInfoStackedWidget;
}

void ProjectManagerWidget::openPage(QString pageName)
{
    QWidget* newWidget = nullptr;

    // Check if the widget with the specified pageName is already added
    for (int i = 0; i < projectInfoStackedWidget->count(); ++i) {
        if (projectInfoStackedWidget->widget(i)->objectName() == pageName) {
            newWidget = projectInfoStackedWidget->widget(i);
            break;
        }
    }

    // If widget not added, create and add it to the stack
    if (!newWidget) {
        if (pageName == "NewProjectPage") {
            newProjectWidget = new NewProjectWidget(this);

            // handle new project widget button actions
            connect(newProjectWidget, &NewProjectWidget::openCreateProject, this, &ProjectManagerWidget::handleOpenCreateProject);
            connect(newProjectWidget, &NewProjectWidget::openJoinProject, this, &ProjectManagerWidget::handleOpenJoinProject);

            newWidget = newProjectWidget;

        } else if (pageName == "CreateProjectPage") {
            createProjectWidget = new CreateProjectWidget(mUser, this);

            // handle create project widget button actions
            connect(createProjectWidget, &CreateProjectWidget::ProjectCreated, this, &ProjectManagerWidget::handleCreateProject);

            newWidget = createProjectWidget;

        } else if (pageName == "JoinProjectPage") {
            joinProjectWidget = new JoinProjectWidget(this);

            newWidget = joinProjectWidget;
        }

        newWidget->setObjectName(pageName);
        projectInfoStackedWidget->addWidget(newWidget);
    }

    // Set the current widget
    projectInfoStackedWidget->setCurrentWidget(newWidget);
}

void ProjectManagerWidget::openDefaultPage()
{
    openPage("defaultProjectPage");
}

void ProjectManagerWidget::handleOpenCreateProject()
{
    openPage("CreateProjectPage");
}

void ProjectManagerWidget::handleOpenJoinProject()
{
    openPage("JoinProjectPage");
}

void ProjectManagerWidget::on_pb_NewProject_clicked()
{
    openPage("NewProjectPage");

}

void ProjectManagerWidget::handleOnProjectButtonClicked(const Project &project)
{
    emit onProjectButtonClicked(project);
}

void ProjectManagerWidget::handleCreateProject()
{
    openDefaultPage();
    createProjectButtons();
}

void ProjectManagerWidget::createProjectButtons()
{
    QLayout *existingLayout = ui->projectScrollAreaWidgetContents->layout();
    if (existingLayout) {
        delete existingLayout;
    }

    projectListLayout = new QVBoxLayout(ui->projectScrollAreaWidgetContents);

    QList<Project> projects = getProjectsFromDatabase();

    for (const auto& project : projects)
    {
        ProjectButton* projectButton = new ProjectButton(project);
        connect(projectButton, &ProjectButton::onProjectButtonClicked, this, &ProjectManagerWidget::handleOnProjectButtonClicked);


        projectListLayout->addWidget(projectButton);
    }

    // design layout
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    projectListLayout->addItem(verticalSpacer);


    ui->projectScrollAreaWidgetContents->setLayout(projectListLayout);
}

QList<Project> ProjectManagerWidget::getProjectsFromDatabase()
{

    // connect to db
    QString error;
    if(!mDbConnection.openDatabase(&error)) {
        qDebug() << "Falied to open database: " << error;
        mDbConnection.closeDatabase();
        return QList<Project>();
    }

    QList<Project> projects;

    QSqlQuery query(QSqlDatabase::database(mDbConnectionName));

    if (!query.prepare("SELECT * FROM projects WHERE user_id = :user_id")) {
        qDebug() << "projectManager ERROR: preparing query";
        mDbConnection.closeDatabase();
        return QList<Project>();
    }
    query.bindValue(":user_id", mUser.getId());

    if (query.exec()) {
        while (query.next()) {
            // Retrieve project details from columns
            int projectId = query.value("prj_id").toInt();
            QString projectTitle = query.value("prj_title").toString();
            QString projectSummary = query.value("prj_summary").toString();
            QString projectDescription = query.value("prj_description").toString();
            int user_id = query.value("user_id").toInt();
            QDateTime projectCreatedAt = query.value("prj_created_at").toDateTime();
            QDateTime projectUpdatedAt = query.value("prj_updated_at").toDateTime();

            // Create a Project object and add it to the list
            Project project(projectId, projectTitle, projectSummary,
                            projectDescription, user_id, projectCreatedAt, projectUpdatedAt);

            projects.append(project);
        }
    } else {
        qDebug() << "Query execution error: " << query.lastError().text();
    }

    mDbConnection.closeDatabase();
    return projects;
}

