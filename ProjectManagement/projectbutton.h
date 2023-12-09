#ifndef PROJECTBUTTON_H
#define PROJECTBUTTON_H

#include "project.h"

#include <QPushButton>

class ProjectButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ProjectButton(const Project& project, QPushButton *parent = nullptr);

signals:
    void onProjectButtonClicked(const Project& project);

private slots:
    void handleOnProjectButtonClicked();

private:
    /*VARIABLES*/

    // project info
    Project mProject;

    /*FUNCTIONS*/

    // setup button ui
    void setupButton();
};

#endif // PROJECTBUTTON_H
