#ifndef REDMINEINSTANCE_H
#define REDMINEINSTANCE_H

#include <QSharedPointer>
#include <QMap>

#include "redmineobjects.h"

//!
//! \brief Синглтон с объектами Redmine
//!
class RedmineInstance
{
public:
    static RedmineInstance &instance ();
    static void deleteInstance ();

    bool loadProjects ();
    const QMap<QString, QSharedPointer<RedmineProject> > &projects ();

    bool loadIssues (const QString &prjid);


    bool loadIssueStatuses ();
    const QMap<QString, QSharedPointer<RedmineIssueStatuses> > &statuses ();

    bool loadIssueCategories ();
    const QMap<QString, QSharedPointer<RedmineIssueCategories> > &categories ();

    bool loadTrackers ();
    const QMap<QString, QSharedPointer<RedmineTrackers> > &trackers ();

protected:

    RedmineInstance ();
    virtual ~RedmineInstance ();

    static RedmineInstance *_instance;

    QMap<QString, QSharedPointer<RedmineProject> > _projects;
    QMap<QString, QSharedPointer<RedmineIssueStatuses> > _statuses;
    QMap<QString, QSharedPointer<RedmineIssueCategories> > _categories;
    QMap<QString, QSharedPointer<RedmineTrackers> > _trackers;
};

#endif // REDMINEINSTANCE_H
