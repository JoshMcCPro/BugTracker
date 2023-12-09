#include "project.h"

#include <QDateTime>

Project::Project(int id, QString title, QString summary,
                 QString description, int user_id, QDateTime createdAt, QDateTime updatedAt)
    : id(id), title(title), summary(summary),
    description(description), user_id(user_id),
    createdAt(createdAt), updatedAt(updatedAt)
{

}

// GETTERS
int Project::getId() const
{
    return this->id;
}

QString Project::getTitle() const
{
    return this->title;
}

QString Project::getSummary() const
{
    return this->summary;
}

QString Project::getDescription() const
{
    return this->description;
}

int Project::getUserId() const
{
    return this->user_id;
}

QDateTime Project::getCreatedAt() const
{
    return this->createdAt;
}

QDateTime Project::getUpdatedAt() const
{
    return this->updatedAt;
}
