#include "projectswidget.h"

#include <QFile>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMenu>

ProjectsWidget::ProjectsWidget (QWidget *parent)
    : QWidget (parent)
    , _view (nullptr)
    , _model (nullptr)
{
    QVBoxLayout *vbl = new QVBoxLayout ();
    vbl->setContentsMargins (4, 4, 4, 4);
    setLayout (vbl);

    _view = new QTreeView (this);
    _view->header ()->hide ();
    _view->setContextMenuPolicy (Qt::CustomContextMenu);
    connect (_view, SIGNAL(customContextMenuRequested(QPoint)),
             this, SLOT(slotCustomContextMenu(QPoint)));
    vbl->addWidget (_view);

    _model = new QStandardItemModel (0, 1);
    _view->setModel (_model);
}

void ProjectsWidget::slotInitProjectsList ()
{
    if (!RedmineInstance::instance ().loadIssueStatuses ()) {
        qCritical () << "[ProjectsWidget][slotInitProjectsList] Could not load issue statuses";
        return;
    }

    if (!RedmineInstance::instance ().loadProjects ()) {
        qCritical () << "[ProjectsWidget][slotInitProjectsList] Could not load projects list";
        return;
    }

    const QMap<QString, QSharedPointer<RedmineProject> > projects =
            RedmineInstance::instance ().projects ();

    _model->removeRows (0, _model->rowCount ());
    _model->setRowCount (0);

    QMapIterator<QString, QSharedPointer<RedmineProject>> iter (projects);
    while (iter.hasNext ()) {
        iter.next ();
        if (iter.value ()->_parent_id == "0")
        {
            QStandardItem *item = new QStandardItem (iter.value ()->_name);
            item->setData (iter.value ()->_id, Qt::UserRole + 1);
            deepTreeRun (item, projects);

            _model->appendRow (item);
        }
    }

    _view->expandAll ();

    //    QMapIterator<QString, QSharedPointer<RedmineProject> > iter
    //            (projects);

    //    while (iter.hasNext ()) {
    //        iter.next ();
    //        qDebug () << iter.value ()->_id << iter.value ()->_parent_id;
    //    }

    //    QSqlDatabase db = QSqlDatabase::database ("yuliya");
    //    if (!db.isValid () || !db.isOpen ())
    //        return;

    //    QSqlQuery query (db);
    //    if (!query.exec ("select * from projects;"))
    //    {
    //        qCritical () << "[ProjectsWidget][slotInitProjectsList] Connection error";
    //        qCritical () << "[ProjectsWidget][slotInitProjectsList]"
    //                     << query.lastError ().databaseText ();
    //        qCritical () << "[ProjectsWidget][slotInitProjectsList]" <<
    //                        query.lastError ().driverText ();
    //        qCritical () << "[ProjectsWidget][slotInitProjectsList]" <<
    //                        query.lastError ().text ();

    //        return;
    //    }

    //    QFile f ("/home/comrade/projects.list");
    //    if (!f.open (QIODevice::ReadWrite | QIODevice::Truncate))
    //        return;

    //    int count = query.record ().count ();
    //    for (int i = 0; i < count; ++i) {
    //        f.write (query.record ().fieldName (i).toUtf8 ());
    //        f.write ("\n");
    //    }
    //    f.close ();
}

void ProjectsWidget::slotCustomContextMenu (const QPoint &pos)
{
    QModelIndex index = _view->indexAt (pos);
    if (!index.isValid ())
        return;

    QString prjid = index.data (Qt::UserRole + 1).toString ();
    if (prjid.isEmpty ())
        return;

    QMenu menu;
    menu.addAction (trUtf8 ("Задачи"));
    QAction *act = menu.exec (_view->viewport ()->mapToGlobal (pos));
    if (!act) return;

    if (act->text () == trUtf8 ("Задачи"))
    {
        if (!RedmineInstance::instance ().loadIssues (prjid)) {
            qCritical () << "[ProjectsWidget][slotCustomContextMenu]";
            return;
        } else {
            QSharedPointer<RedmineProject> project =
                RedmineInstance::instance ().projects ().value (prjid);
            if (project.isNull ())
                return;
            for (int i = 0; i < project->_issues.size (); ++i)
                qDebug () << project->_issues[i]->_subject
                          << RedmineInstance::instance ().statuses ()[project->_issues[i]->_status_id]->_name;
        }
    }
}

void ProjectsWidget::showEvent (QShowEvent */*ev*/)
{
    QSqlDatabase db = QSqlDatabase::database ("yuliya");
    if (!db.isValid () || !db.isOpen ())
        return;

    QTimer::singleShot (0, this, SLOT(slotInitProjectsList()));
}

void ProjectsWidget::deepTreeRun (QStandardItem *item,
                                  const QMap<QString, QSharedPointer<RedmineProject> > &prjs)
{
    if (prjs.isEmpty ())
        return;

    QMapIterator<QString, QSharedPointer<RedmineProject>> iter (prjs);
    while (iter.hasNext ())
    {
        iter.next ();
        if (iter.value ()->_parent_id == item->data (Qt::UserRole + 1).toString ())
        {
            QStandardItem *child = new QStandardItem (iter.value ()->_name);
            child->setData (iter.value ()->_id, Qt::UserRole + 1);
            deepTreeRun (child, prjs);

            item->appendRow (child);
        }
    }
}
