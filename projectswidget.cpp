#include "projectswidget.h"
#include "projectsettingsdialog.h"

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
    _view->setSelectionMode (QAbstractItemView::SingleSelection);
    _view->setEditTriggers (QAbstractItemView::NoEditTriggers);

    _model = new QStandardItemModel (0, 1);
    _view->setModel (_model);

    connect (_view, SIGNAL(customContextMenuRequested(QPoint)),
             this, SLOT(slotCustomContextMenu(QPoint)));
    connect (_view->selectionModel (), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             SLOT(slotSelectionChanged(QItemSelection,QItemSelection)));
    vbl->addWidget (_view);
}

QString ProjectsWidget::selectedProject () const
{
    if (_currentIndex.isValid ())
        return _currentIndex.data (Qt::UserRole + 1).toString ();
    else
        return QString::null;
}

void ProjectsWidget::slotInitProjectsList ()
{
    if (RedmineInstance::instance ().loadIssueCategories ()) {
        qDebug () << "[ProjectsWidget][slotInitProjectsList] Issue categories loaded";
    } else {
        qCritical () << "[ProjectsWidget][slotInitProjectsList] Could not load issue categories";
        return;
    }

    if (RedmineInstance::instance ().loadIssueStatuses ()) {
        qDebug () << "[ProjectsWidget][slotInitProjectsList] Issue statuses loaded";
    } else {
        qCritical () << "[ProjectsWidget][slotInitProjectsList] Could not load issue statuses";
        return;
    }

    if (RedmineInstance::instance ().loadTrackers ()) {
        qDebug () << "[ProjectsWidget][slotInitProjectsList] Trackers loaded";
    } else {
        qCritical () << "[ProjectsWidget][slotInitProjectsList] Could not load trackers";
        return;
    }

    if (RedmineInstance::instance ().loadProjects ()) {
        qDebug () << "[ProjectsWidget][slotInitProjectsList] Projects loaded";
    } else {
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
    menu.addAction (trUtf8 ("Настройки"));
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
                //qDebug () << project->_issues[i]->_subject
                //          << RedmineInstance::instance ().statuses ()[project->_issues[i]->_status_id]->_name
                //          << project->_issues[i]->_category_id;
                if (trUtf8 ("При установке текущей Ген.Сх. удаляются другие") == project->_issues[i]->_subject)
                {
                    qDebug () << project->_issues[i]->_assigned_to_id << "_assigned_to_id";
                    qDebug () << project->_issues[i]->_author_id << "_author_id";
                    qDebug () << project->_issues[i]->_category_id << "_category_id";
                    qDebug () << project->_issues[i]->_closed_on << "_closed_on";
                    qDebug () << project->_issues[i]->_closed_on << "_closed_on";
                    qDebug () << project->_issues[i]->_created_on << "_created_on";
                    qDebug () << project->_issues[i]->_description << "_description";
                    qDebug () << project->_issues[i]->_done_ratio << "_done_ratio";
                    qDebug () << project->_issues[i]->_due_date << "_due_date";
                    qDebug () << project->_issues[i]->_estimated_hours << "_estimated_hours";
                    qDebug () << project->_issues[i]->_fixed_version_id << "_fixed_version_id";
                    qDebug () << project->_issues[i]->_id << "_id";
                    qDebug () << project->_issues[i]->_is_private << "_is_private";
                    qDebug () << project->_issues[i]->_lft << "_lft";
                    qDebug () << project->_issues[i]->_lock_version << "_lock_version";
                    qDebug () << project->_issues[i]->_parent_id << "_parent_id";
                    qDebug () << project->_issues[i]->_priority_id << "_priority_id";
                    qDebug () << project->_issues[i]->_rgt << "_rgt";
                    qDebug () << project->_issues[i]->_root_id << "_root_id";
                    qDebug () << project->_issues[i]->_start_date << "_start_date";
                    qDebug () << project->_issues[i]->_status_id << "_status_id";
                    qDebug () << project->_issues[i]->_subject << "_subject";
                    qDebug () << project->_issues[i]->_tracker_id << "_tracker_id";
                    qDebug () << project->_issues[i]->_updated_on << "_updated_on";
                }
        }
    }
    else if (act->text () == trUtf8 ("Настройки"))
    {
        if (!RedmineInstance::instance ().loadIssues (prjid)) {
            qCritical () << "[ProjectsWidget][slotCustomContextMenu]";
            return;
        } else {
            auto *dlg = new ProjectSettingsDialog (prjid, this);
            dlg->exec ();
        }
    }
}

void ProjectsWidget::slotSelectionChanged (const QItemSelection &selected, const QItemSelection &deselected)
{
    (void)deselected;

    if (selected.isEmpty ())
        _currentIndex = QModelIndex ();
    else
        _currentIndex = selected.indexes ()[0];
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
