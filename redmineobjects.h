#ifndef REDMINEOBJECTS_H
#define REDMINEOBJECTS_H

#include <QString>
#include <QSharedPointer>
#include <QSettings>

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
    RedmineProject ()
    {
        _code_lines = 0;
        _exceptions = 0;
        _incorrect_actions = 0;
        _reference_number_of_error = 0;
        _required_actions = 0;

        readSettings ();
    }

    virtual ~RedmineProject ()
    {
        writeSettings ();
    }

    void readSettings ()
    {
        if (_id.isEmpty ())
            return;

        QSettings settings ("yuliya", "yuliya");
        settings.beginGroup (QString ("project/%1").arg (_id));

        _code_lines = settings.value ("code_lines", "0").toInt ();
        _exceptions = settings.value ("exceptions", "0").toInt ();
        _incorrect_actions = settings.value ("incorrect_actions", "0").toInt ();
        _reference_number_of_error = settings.value ("reference_number_of_error", "0").toInt ();
        _required_actions = settings.value ("required_actions", "0").toInt ();

        settings.endGroup ();
    }

    void writeSettings ()
    {
        if (_id.isEmpty ())
            return;

        QSettings settings ("yuliya", "yuliya");
        settings.beginGroup (QString ("project/%1").arg (_id));

        settings.setValue ("code_lines", QString::number (_code_lines));
        settings.setValue ("exceptions", QString::number (_exceptions));
        settings.setValue ("incorrect_actions", QString::number (_incorrect_actions));
        settings.setValue ("reference_number_of_error", QString::number (_reference_number_of_error));
        settings.setValue ("required_actions", QString::number (_required_actions));

        settings.endGroup ();
    }

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

    int _code_lines;
    int _exceptions;
    int _incorrect_actions;
    int _reference_number_of_error;
    int _required_actions;
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
