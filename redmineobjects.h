#ifndef REDMINEOBJECTS_H
#define REDMINEOBJECTS_H

#include <QString>
#include <QSharedPointer>

struct RedmineIssue
{
    QString _id;
    QString _tracker_id;
    QString _project_id;
    QString _subject;
    QString _description;
    QString _due_date;
    QString _category_id;
    QString _status_id;
    QString _assigned_to_id;
    QString _priority_id;
    QString _fixed_version_id;
    QString _author_id;
    QString _lock_version;
    QString _created_on;
    QString _updated_on;
    QString _start_date;
    QString _done_ratio;
    QString _estimated_hours;
    QString _parent_id;
    QString _root_id;
    QString _lft;
    QString _rgt;
    QString _is_private;
    QString _closed_on;
};

struct RedmineProject
{
    QString _id;
    QString _name;
    QString _description;
    QString _homepage;
    QString _is_public;
    QString _parent_id;
    QString _created_on;
    QString _updated_on;
    QString _identifier;
    QString _status;
    QString _lft;
    QString _rgt;
    QString _inherit_members;
    QString _default_version_id;
    QString _default_assigned_to_id;

    QList<QSharedPointer<RedmineIssue>> _issues;
};

struct RedmineIssueStatuses
{
    QString _id;
    QString _name;
    QString _is_closed;
    QString _position;
    QString _default_done_ratio;
};

#endif // REDMINEOBJECTS_H
