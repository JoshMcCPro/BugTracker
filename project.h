#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QDateTime>

class Project
{
public:
    Project(int id, QString title, QString summary,
            QString description, int user_id, QDateTime createdAt, QDateTime updatedAt);

    // Setters
    //void setTitle(const QString& title);
    //void setSummary(const QString& summary);
    //void setDescription(const QString& description);

    // Getters
    int getId() const;
    QString getTitle() const;
    QString getSummary() const;
    QString getDescription() const;
    int getUserId() const;
    QDateTime getCreatedAt() const;
    QDateTime getUpdatedAt() const;

private:

    /* PROJECT INFO */
    int id;
    QString title;
    QString summary;
    QString description;
    int user_id;
    QDateTime createdAt;
    QDateTime updatedAt;
};

#endif // PROJECT_H
