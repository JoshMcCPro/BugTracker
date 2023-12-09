#include "issuemanager.h"
#include "ui_issuemanager.h"

IssueManager::IssueManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IssueManager)
{
    ui->setupUi(this);
}

IssueManager::~IssueManager()
{
    delete ui;
}
