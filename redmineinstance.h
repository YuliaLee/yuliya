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

protected:
    RedmineInstance ();
    virtual ~RedmineInstance ();

    static RedmineInstance *_instance;

    QMap<QString, QSharedPointer<RedmineProject> > _projects;
};

#endif // REDMINEINSTANCE_H
