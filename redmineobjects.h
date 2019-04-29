#ifndef REDMINEOBJECTS_H
#define REDMINEOBJECTS_H

#include <QString>
#include <QSharedPointer>
#include <QSettings>

struct RedmineProjectCodeMetrics
{
    QString _date;
    QString _code_lines;
    QString _errors;
    QString _test_cases;
};

//!
//! \brief Типы issue - ошибка, улучшение, функционал и т.д.
//!
struct RedmineTrackers
{
    enum {
        Error = 1,
        Feature = 2,
        Support = 3,
        Functional = 4,
        TestCase = 5
    };

    QString _id;
    QString _name;
    QString _is_in_chlog;
    QString _position;
    QString _is_in_roadmap;
    QString _fields_bits;
    QString _default_status_id;
};

struct RedmineIssueCategories
{
    QString _id;
    QString _project_id;
    QString _name;
    QString _assigned_to_id;
};

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

        QSettings settings ("Inteltech", "yuliya");
        settings.beginGroup (QString ("project/%1").arg (_id));

        //_code_lines = settings.value ("code_lines", "0").toInt ();
        _exceptions = settings.value ("exceptions", "0").toInt ();
        _incorrect_actions = settings.value ("incorrect_actions", "0").toInt ();
        _reference_number_of_error = settings.value ("reference_number_of_error", "0").toInt ();
        _required_actions = settings.value ("required_actions", "0").toInt ();
        _need_test_case = settings.value ("need_test_case", "0").toInt ();

        settings.endGroup ();
    }

    void writeSettings ()
    {
        if (_id.isEmpty ())
            return;

        QSettings settings ("Inteltech", "yuliya");
        settings.beginGroup (QString ("project/%1").arg (_id));

        //settings.setValue ("code_lines", QString::number (_code_lines));
        settings.setValue ("exceptions", QString::number (_exceptions));
        settings.setValue ("incorrect_actions", QString::number (_incorrect_actions));
        settings.setValue ("reference_number_of_error", QString::number (_reference_number_of_error));
        settings.setValue ("required_actions", QString::number (_required_actions));
        settings.setValue ("need_test_case", QString::number (_need_test_case));

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

    int _exceptions;                    //!< Требуемое количество иключений
    int _incorrect_actions;             //!< Требуемое количество некорректных действий
    int _reference_number_of_error;     //!< Планируемое количество ошибок
    int _required_actions;
    int _need_test_case;                //!< Количество требуемых тест-кейсов
};

struct RedmineIssueStatuses
{
    QString _id;
    QString _name;
    QString _is_closed;
    QString _position;
    QString _default_done_ratio;
};

struct RedmineAttachments
{
    QString _id;
    QString _container_id;
    QString _container_type;
    QString _filename;
    QString _disk_filename;
    QString _filesize;
    QString _content_type;
    QString _digest;
    QString _downloads;
    QString _author_id;
    QString _created_on;
    QString _description;
    QString _disk_directory;
};

#endif // REDMINEOBJECTS_H
