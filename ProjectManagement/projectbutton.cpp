#include "ProjectManagement\projectbutton.h"
#include "project.h"

#include <QPushButton>

ProjectButton::ProjectButton(const Project& project, QPushButton *parent)
    : QPushButton(parent),
    mProject(project)
{
    setupButton();
    connect(this, &ProjectButton::onProjectButtonclicked, this, &ProjectButton::handleOnProjectButtonClicked);
}

void ProjectButton::handleOnProjectButtonClicked()
{
    // perform project button action
    // go to project page
    emit onProjectButtonclicked(mProject);
}

void ProjectButton::setupButton()
{

    setText(mProject.getTitle());

    QString objectName = (mProject.getTitle() + "Button");
    setObjectName(objectName);
}
