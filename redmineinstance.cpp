#include "redmineinstance.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

RedmineInstance *RedmineInstance::_instance = nullptr;

RedmineInstance &RedmineInstance::instance ()
{
    if (!_instance)
        _instance = new RedmineInstance ();
    return *_instance;
}

void RedmineInstance::deleteInstance ()
{
    if (_instance)
        delete _instance;
    _instance = nullptr;
}

bool RedmineInstance::loadProjects ()
{
    QSqlDatabase db = QSqlDatabase::database ("yuliya");
    if (!db.isValid () || !db.isOpen ()) {
        qCritical () << "[RedmineInstance][loadProjects] Could not find 'yuliya' database";
        return false;
    }

    QSqlQuery query (db);
    if (!query.exec ("select * from projects;"))
    {
        qCritical () << "[RedmineInstance][loadProjects] Connection error";
        qCritical () << "[RedmineInstance][loadProjects]"
                     << query.lastError ().databaseText ();
        qCritical () << "[RedmineInstance][loadProjects]" <<
                        query.lastError ().driverText ();
        qCritical () << "[RedmineInstance][loadProjects`]" <<
                        query.lastError ().text ();

        return false;
    }

    while (query.next ())
    {
        QSharedPointer<RedmineProject> project
                = QSharedPointer<RedmineProject> (new RedmineProject);
        project->_id = query.record ().value ("id").toString ();
        project->_name = query.record ().value ("name").toString ();
        project->_description = query.record ().value ("description").toString ();
        project->_homepage = query.record ().value ("homepage").toString ();
        project->_is_public = query.record ().value ("is_public").toString ();
        project->_parent_id = query.record ().value ("parent_id").toString ();
        project->_created_on = query.record ().value ("created_on").toString ();
        project->_updated_on = query.record ().value ("updated_on").toString ();
        project->_identifier = query.record ().value ("identifier").toString ();
        project->_status = query.record ().value ("status").toString ();
        project->_lft = query.record ().value ("lft").toString ();
        project->_rgt = query.record ().value ("rgt").toString ();
        project->_inherit_members = query.record ().value ("inherit_members").toString ();
        project->_default_version_id = query.record ().value ("default_version_id").toString ();
        project->_default_assigned_to_id = query.record ().value ("default_assigned_to_id").toString ();

        project->readSettings ();

        _projects.insert (project->_id, project);
    }

    return true;
}

const QMap<QString, QSharedPointer<RedmineProject> > &RedmineInstance::projects () {
    return _projects;
}

bool RedmineInstance::loadIssues (const QString &prjid)
{
    if (prjid.isEmpty () || !_projects.contains (prjid))
        return false;

    QSqlDatabase db = QSqlDatabase::database ("yuliya");
    if (!db.isValid () || !db.isOpen ()) {
        qCritical () << "[RedmineInstance][loadIssues] Could not find 'yuliya' database";
        return false;
    }

    QSqlQuery query (db);
    if (!query.exec (QString ("SELECT * FROM issues WHERE project_id='%1';").arg (prjid)))
    {
        qCritical () << "[RedmineInstance][loadIssues] Connection error";
        qCritical () << "[RedmineInstance][loadIssues]"
                     << query.lastError ().databaseText ();
        qCritical () << "[RedmineInstance][loadIssues]" <<
                        query.lastError ().driverText ();
        qCritical () << "[RedmineInstance][loadIssues`]" <<
                        query.lastError ().text ();

        return false;
    }

    _projects[prjid]->_issues.clear ();

    while (query.next ())
    {
        QSharedPointer<RedmineIssue> issue =
                QSharedPointer<RedmineIssue> (new RedmineIssue);
        issue->_id = query.record ().value ("id").toString ();
        issue->_tracker_id = query.record ().value ("tracker_id").toString ();
        issue->_project_id = query.record ().value ("project_id").toString ();
        issue->_subject = query.record ().value ("subject").toString ();
        issue->_description = query.record ().value ("description").toString ();
        issue->_due_date = query.record ().value ("due_date").toString ();
        issue->_category_id = query.record ().value ("category_id").toString ();
        issue->_status_id = query.record ().value ("status_id").toString ();
        issue->_assigned_to_id = query.record ().value ("assigned_to_id").toString ();
        issue->_priority_id = query.record ().value ("priority_id").toString ();
        issue->_fixed_version_id = query.record ().value ("fixed_version_id").toString ();
        issue->_author_id = query.record ().value ("author_id").toString ();
        issue->_lock_version = query.record ().value ("lock_version").toString ();
        issue->_created_on = query.record ().value ("created_on").toString ();
        issue->_updated_on = query.record ().value ("updated_on").toString ();
        issue->_start_date = query.record ().value ("start_date").toString ();
        issue->_done_ratio = query.record ().value ("done_ratio").toString ();
        issue->_estimated_hours = query.record ().value ("estimated_hours").toString ();
        issue->_parent_id = query.record ().value ("parent_id").toString ();
        issue->_root_id = query.record ().value ("root_id").toString ();
        issue->_lft = query.record ().value ("lft").toString ();
        issue->_rgt = query.record ().value ("rgt").toString ();
        issue->_is_private = query.record ().value ("is_private").toString ();
        issue->_closed_on = query.record ().value ("closed_on").toString ();

        _projects[prjid]->_issues.append (issue);
    }

    return true;
}

bool RedmineInstance::loadIssueStatuses ()
{
    QSqlDatabase db = QSqlDatabase::database ("yuliya");
    if (!db.isValid () || !db.isOpen ()) {
        qCritical () << "[RedmineInstance][loadIssueStatuses] Could not find 'yuliya' database";
        return false;
    }

    QSqlQuery query (db);
    if (!query.exec ("SELECT * FROM issue_statuses;"))
    {
        qCritical () << "[RedmineInstance][loadIssueStatuses] Connection error";
        qCritical () << "[RedmineInstance][loadIssueStatuses]"
                     << query.lastError ().databaseText ();
        qCritical () << "[RedmineInstance][loadIssueStatuses]" <<
                        query.lastError ().driverText ();
        qCritical () << "[RedmineInstance][loadIssueStatuses]" <<
                        query.lastError ().text ();

        return false;
    }

    while (query.next ())
    {
        QSharedPointer<RedmineIssueStatuses> status
                = QSharedPointer<RedmineIssueStatuses> (new RedmineIssueStatuses);
        status->_id = query.record ().value ("id").toString ();
        status->_default_done_ratio = query.record ().value ("default_done_ratio").toString ();
        status->_is_closed = query.record ().value ("is_closed").toString ();
        status->_name = query.record ().value ("name").toString ();
        status->_position = query.record ().value ("position").toString ();

        _statuses.insert (status->_id, status);
    }

    return true;
}

const QMap<QString, QSharedPointer<RedmineIssueStatuses> > &RedmineInstance::statuses () {
    return _statuses;
}

bool RedmineInstance::loadIssueCategories ()
{
    QSqlDatabase db = QSqlDatabase::database ("yuliya");
    if (!db.isValid () || !db.isOpen ()) {
        qCritical () << "[RedmineInstance][loadIssueCategories] Could not find 'yuliya' database";
        return false;
    }

    QSqlQuery query (db);
    if (!query.exec ("SELECT * FROM issue_categories;"))
    {
        qCritical () << "[RedmineInstance][loadIssueCategories] Connection error";
        qCritical () << "[RedmineInstance][loadIssueCategories]"
                     << query.lastError ().databaseText ();
        qCritical () << "[RedmineInstance][loadIssueCategories]" <<
                        query.lastError ().driverText ();
        qCritical () << "[RedmineInstance][loadIssueCategories]" <<
                        query.lastError ().text ();

        return false;
    }

    while (query.next ())
    {
        QSharedPointer<RedmineIssueCategories> cat
                = QSharedPointer<RedmineIssueCategories> (new RedmineIssueCategories);
        cat->_id = query.record ().value ("id").toString ();
        cat->_project_id = query.record ().value ("project_id").toString ();
        cat->_name = query.record ().value ("name").toString ();
        cat->_assigned_to_id = query.record ().value ("assigned_to_id").toString ();

        _categories.insert (cat->_id, cat);
    }

    return true;
}

const QMap<QString, QSharedPointer<RedmineIssueCategories> > &RedmineInstance::categories () {
    return _categories;
}

bool RedmineInstance::loadTrackers ()
{
    QSqlDatabase db = QSqlDatabase::database ("yuliya");
    if (!db.isValid () || !db.isOpen ()) {
        qCritical () << "[RedmineInstance][loadTrackers] Could not find 'yuliya' database";
        return false;
    }

    QSqlQuery query (db);
    if (!query.exec ("SELECT * FROM trackers;"))
    {
        qCritical () << "[RedmineInstance][loadTrackers] Connection error";
        qCritical () << "[RedmineInstance][loadTrackers]"
                     << query.lastError ().databaseText ();
        qCritical () << "[RedmineInstance][loadTrackers]" <<
                        query.lastError ().driverText ();
        qCritical () << "[RedmineInstance][loadTrackers]" <<
                        query.lastError ().text ();

        return false;
    }

    while (query.next ())
    {
        QSharedPointer<RedmineTrackers> tracker
                = QSharedPointer<RedmineTrackers> (new RedmineTrackers);
        tracker->_id = query.record ().value ("id").toString ();
        tracker->_name = query.record ().value ("name").toString ();
        tracker->_is_in_chlog = query.record ().value ("is_in_chlog").toString ();
        tracker->_position = query.record ().value ("position").toString ();
        tracker->_is_in_roadmap = query.record ().value ("is_in_roadmap").toString ();
        tracker->_fields_bits = query.record ().value ("fields_bits").toString ();
        tracker->_default_status_id = query.record ().value ("default_status_id").toString ();

        _trackers.insert (tracker->_id, tracker);
    }

    return true;
}

const QMap<QString, QSharedPointer<RedmineTrackers> > &RedmineInstance::trackers () {
    return _trackers;
}

bool RedmineInstance::loadAttachments ()
{
    QSqlDatabase db = QSqlDatabase::database ("yuliya");
    if (!db.isValid () || !db.isOpen ()) {
        qCritical () << "[RedmineInstance][loadAttachments] Could not find 'yuliya' database";
        return false;
    }

    QSqlQuery query (db);
    if (!query.exec ("SELECT * FROM attachments;"))
    {
        qCritical () << "[RedmineInstance][loadAttachments] Connection error";
        qCritical () << "[RedmineInstance][loadAttachments]"
                     << query.lastError ().databaseText ();
        qCritical () << "[RedmineInstance][loadAttachments]" <<
                        query.lastError ().driverText ();
        qCritical () << "[RedmineInstance][loadAttachments]" <<
                        query.lastError ().text ();

        return false;
    }

    while (query.next ())
    {
        QSharedPointer<RedmineAttachments> attach
                = QSharedPointer<RedmineAttachments> (new RedmineAttachments);

        attach->_id = query.record ().value ("id").toString ();
        attach->_container_id = query.record ().value ("container_id").toString ();
        attach->_container_type = query.record ().value ("container_type").toString ();

        attach->_filename = query.record ().value ("filename").toString ();
        attach->_disk_filename = query.record ().value ("disk_filename").toString ();
        attach->_filesize = query.record ().value ("filesize").toString ();
        attach->_content_type = query.record ().value ("content_type").toString ();
        attach->_digest = query.record ().value ("digest").toString ();
        attach->_downloads = query.record ().value ("downloads").toString ();
        attach->_author_id = query.record ().value ("author_id").toString ();
        attach->_created_on = query.record ().value ("created_on").toString ();
        attach->_description = query.record ().value ("description").toString ();
        attach->_disk_directory = query.record ().value ("disk_directory").toString ();

        _attachments.insert (attach->_id, attach);
    }

    return true;
}

const QMap<QString, QSharedPointer<RedmineAttachments> > &RedmineInstance::attachments () {
    return _attachments;
}

const QList<RedmineProjectCodeMetrics> RedmineInstance::codeMetrics(const QString &prjid)
{
    QList<RedmineProjectCodeMetrics> code_metrics;

    QSettings settings ("Inteltech", "yuliya");
    settings.beginGroup (QString ("code_settings_%1").arg (prjid));

    QStringList lst = settings.allKeys ();
    for (int i = 0; i < lst.size (); ++i)
    {
        QStringList values = settings.value (lst[i]).toString ().split (";");

        RedmineProjectCodeMetrics metric;
        metric._date = lst[i];
        metric._code_lines = values[0];
        metric._errors = values[1];
        metric._test_cases = values[2];

        code_metrics.append (metric);
    }
    settings.endGroup ();

    return code_metrics;
}

int RedmineInstance::metric1A (const QString &prjid)
{
    int A = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем задачи для предотращения некорректных действий
        if (project->_issues[i]->_tracker_id == "4" &&          //функционал
                (project->_issues[i]->_category_id == "37" ||
                 project->_issues[i]->_category_id == "38" ||
                 project->_issues[i]->_category_id == "39" ||
                 project->_issues[i]->_category_id == "47" ||
                 project->_issues[i]->_category_id == "48" ||
                 project->_issues[i]->_category_id == "49" ||
                 project->_issues[i]->_category_id == "50" ||
                 project->_issues[i]->_category_id == "51" ||
                 project->_issues[i]->_category_id == "52" ||
                 project->_issues[i]->_category_id == "53") &&  //категория check test
                (project->_issues[i]->_priority_id == "4" ||    //приоритет - срочный или немедленный
                 project->_issues[i]->_priority_id == "5") &&
                 project->_issues[i]->_status_id == "3")        //статус - решена
        {
            A++;
        }
    }

    return A;
}

int RedmineInstance::metric1B (const QString &prjid)
{
    int B = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем задачи для предотращения некорректных действий
        if (project->_issues[i]->_tracker_id == "4" &&          //функционал
                (project->_issues[i]->_category_id == "37" ||
                 project->_issues[i]->_category_id == "38" ||
                 project->_issues[i]->_category_id == "39" ||
                 project->_issues[i]->_category_id == "47" ||
                 project->_issues[i]->_category_id == "48" ||
                 project->_issues[i]->_category_id == "49" ||
                 project->_issues[i]->_category_id == "50" ||
                 project->_issues[i]->_category_id == "51" ||
                 project->_issues[i]->_category_id == "52" ||
                 project->_issues[i]->_category_id == "53") &&  //категория check test
                (project->_issues[i]->_priority_id == "4" ||    //приоритет - срочный или немедленный
                 project->_issues[i]->_priority_id == "5"))
        {
            B++;
        }
    }

    return B;
}

int RedmineInstance::metric2A (const QString &prjid)
{
    int A = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все решенные exception
        if (project->_issues[i]->_tracker_id == "4" &&          //функционал
                (project->_issues[i]->_category_id == "31" ||
                 project->_issues[i]->_category_id == "32" ||
                 project->_issues[i]->_category_id == "33" ||
                 project->_issues[i]->_category_id == "36") &&
                (project->_issues[i]->_priority_id == "4" ||    //приоритет - срочный или немедленный
                 project->_issues[i]->_priority_id == "5") &&   //категория exception
                 project->_issues[i]->_status_id == "3")        //статус - решена
        {
            A++;
        }
    }

    return A;
}

int RedmineInstance::metric2B (const QString &prjid)
{
    int B = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все ошибки
        if (project->_issues[i]->_tracker_id == "4" &&          //функционал
                (project->_issues[i]->_category_id == "31" ||
                 project->_issues[i]->_category_id == "32" ||
                 project->_issues[i]->_category_id == "33" ||
                 project->_issues[i]->_category_id == "36") &&  //категория exception
                (project->_issues[i]->_priority_id == "4" ||    //приоритет - срочный или немедленный
                 project->_issues[i]->_priority_id == "5"))
        {
            B++;
        }
    }

    return B;
}

int RedmineInstance::metric3A (const QString &prjid)
{
    int A = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все ошибки
        if (project->_issues[i]->_tracker_id == "1")
            A++;
    }

    return A;
}

int RedmineInstance::metric3B (const QString &prjid)
{
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    return project->_reference_number_of_error;
}

int RedmineInstance::metric4A (const QString &prjid)
{
    int A = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все закрытые ошибки
        if (project->_issues[i]->_tracker_id == "1" &&
                project->_issues[i]->_status_id == "3")
            A++;
    }

    return A;
}

int RedmineInstance::metric4B (const QString &prjid)
{
    int B = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все ошибки
        if (project->_issues[i]->_tracker_id == "1")
            B++;
    }

    return B;
}

int RedmineInstance::metric5A (const QString &prjid)
{
    int A = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //ищем все разработанные тест-кейсы
        if (project->_issues[i]->_tracker_id == "5" &&
                project->_issues[i]->_status_id == "8")
            A++;
    }
    return A;
}

int RedmineInstance::metric5B (const QString &prjid)
{
    int B = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //ищем все тест-кейсы
        if (project->_issues[i]->_tracker_id == "5")
            B++;
    }
    return B;
}

int RedmineInstance::metric6A (const QString &prjid)
{
    int A = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все решенные исключения
        if (project->_issues[i]->_tracker_id == "4" &&    //функционал
            project->_issues[i]->_category_id == "54" &&  //категория Restorability Test
            project->_issues[i]->_status_id == "3")       //статус - решена
        {
            A++;
        }
    }

    return A;
}

int RedmineInstance::metric6B (const QString &prjid)
{
    int B = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все исключения
        if (project->_issues[i]->_tracker_id == "4" &&    //функционал
            project->_issues[i]->_category_id == "54")    //категория Restorability Test
        {
            B++;
        }
    }

    return B;
}

int RedmineInstance::metric7A (const QString &prjid)
{
    int A = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все ошибки
        if (project->_issues[i]->_tracker_id == "4" &&          //функционал
                project->_issues[i]->_status_id == "3")         //статус - решена
        {
            QList<QSharedPointer<RedmineAttachments> > attachments
                    = RedmineInstance::instance ().attachments ().values ();

            for (int j = 0; j < attachments.size (); ++j)
            {
                if (attachments[j]->_container_type == "Issue" &&
                        attachments[j]->_container_id == project->_issues[i]->_id &&
                        (attachments[j]->_filename.toLower ().contains (QObject::trUtf8 ("надёжност")) ||
                         attachments[j]->_filename.toLower ().contains (QObject::trUtf8 ("надежност"))))
                    A++;
            }
        }
    }

    return A;
}

int RedmineInstance::metric7B (const QString &prjid)
{
    int B = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все ошибки
        if (project->_issues[i]->_tracker_id == "4")          //функционал
        {
            QList<QSharedPointer<RedmineAttachments> > attachments
                    = RedmineInstance::instance ().attachments ().values ();

            for (int j = 0; j < attachments.size (); ++j)
            {
                if (attachments[j]->_container_type == "Issue" &&
                        attachments[j]->_container_id == project->_issues[i]->_id &&
                        (attachments[j]->_filename.toLower ().contains (QObject::trUtf8 ("надёжност")) ||
                         attachments[j]->_filename.toLower ().contains (QObject::trUtf8 ("надежност"))))
                    B++;
            }
        }
    }

    return B;
}

int RedmineInstance::metric8A (const QString &prjid)
{
    int A = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все ошибки
        if (project->_issues[i]->_tracker_id == "1" &&
                (project->_issues[i]->_priority_id == "3" ||
                 project->_issues[i]->_priority_id == "4" ||
                 project->_issues[i]->_priority_id == "5"))
        {
            A++;
        }
    }

    return A;
}

int RedmineInstance::metric8B (const QString &prjid)
{
    int B = 0;
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
    for (int i = 0; i < project->_issues.size (); ++i)
    {
        //-- ищем все ошибки
        if (project->_issues[i]->_tracker_id == "1")
        {
            B++;
        }
    }

    return B;
}

RedmineInstance::RedmineInstance ()
{}

RedmineInstance::~RedmineInstance ()
{}
