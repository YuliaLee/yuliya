#ifndef REDMINEOBJECTS_H
#define REDMINEOBJECTS_H

#include <QString>

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
};

#endif // REDMINEOBJECTS_H
