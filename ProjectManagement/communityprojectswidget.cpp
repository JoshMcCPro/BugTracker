#include "communityprojectswidget.h"
#include "ui_communityprojectswidget.h"

CommunityProjectsWidget::CommunityProjectsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunityProjectsWidget)
{
    ui->setupUi(this);
}

CommunityProjectsWidget::~CommunityProjectsWidget()
{
    delete ui;
}
