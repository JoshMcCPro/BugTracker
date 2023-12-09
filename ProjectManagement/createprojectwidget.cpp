#include "createprojectwidget.h"
#include "ui_createprojectwidget.h"
#include "databaseconnection.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

CreateProjectWidget::CreateProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateProjectWidget),
    mDbConnection("serverbugtracker.database.windows.net",
                  "SQL SERVER",
                  "JoshMcCPro",
                  "Jam8122233302!",
                  "DB_BUGTRACKER",
                  mDbConnectionName)
{
    ui->setupUi(this);
}

CreateProjectWidget::~CreateProjectWidget()
{
    delete ui;
}

void CreateProjectWidget::createProject()
{
    QString title = ui->le_Title->text();
    QString summary = ui->le_Summary->text();
    QString description = ui->pte_Description->toPlainText();

    QString error;
    if (!mDbConnection.openDatabase(&error)) {
        qDebug() << "Database connection error: " << error;
        return;
    }

    QSqlQuery query(QSqlDatabase::database(mDbConnectionName));

    if (!query.prepare("INSERT INTO projects (prj_title, prj_summary, prj_description, prj_created_at)"
                       "VALUES (:title, :summary, :description, :createdAt)")) {
        qDebug() << "Error preparing project query: " << query.lastError().text();
        QSqlDatabase::database(mDbConnectionName).rollback();
        return;
    }

    query.bindValue(":title", title);
    query.bindValue(":summary", summary);
    query.bindValue(":description", description);
    query.bindValue(":createdAt", QDateTime::currentDateTime());


    if (!query.exec()) {
        qDebug() << "Error executing project query: " << query.lastError().text();
        QSqlDatabase::database(mDbConnectionName).rollback();
        return;
    }

    // Get the project ID of the newly inserted project
    int projectId = query.lastInsertId().toInt();

    qDebug() << "Project info sent to database. Project ID: " << projectId;

    // Commit the transaction
    QSqlDatabase::database(mDbConnectionName).commit();

    mDbConnection.closeDatabase();

    emit ProjectCreated();

}

void CreateProjectWidget::on_pb_Create_clicked()
{
    createProject();
}

