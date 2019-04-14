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

        _projects.insert (project->_id, project);
    }

    return true;
}

const QMap<QString, QSharedPointer<RedmineProject> > &RedmineInstance::projects () {
    return _projects;
}

RedmineInstance::RedmineInstance ()
{

}

RedmineInstance::~RedmineInstance ()
{

}
