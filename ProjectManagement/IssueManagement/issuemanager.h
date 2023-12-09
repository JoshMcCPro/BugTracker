#ifndef ISSUEMANAGER_H
#define ISSUEMANAGER_H

#include <QWidget>

namespace Ui {
class IssueManager;
}

class IssueManager : public QWidget
{
    Q_OBJECT

public:
    explicit IssueManager(QWidget *parent = nullptr);
    ~IssueManager();

private:
    Ui::IssueManager *ui;
};

#endif // ISSUEMANAGER_H
