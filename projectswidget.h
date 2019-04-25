#ifndef PROJECTSWIDGET_H
#define PROJECTSWIDGET_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>

#include "redmineinstance.h"

class ProjectsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectsWidget (QWidget *parent = nullptr);
    QString selectedProject () const;

signals:
    void projectSelected (const QString &prjid);

protected slots:
    void slotInitProjectsList ();
    void slotCustomContextMenu (const QPoint &pos);
    void slotSelectionChanged (const QItemSelection &selected, const QItemSelection &deselected);

protected:
    void showEvent (QShowEvent *ev);

    void deepTreeRun (QStandardItem *item,
                      const QMap<QString, QSharedPointer<RedmineProject> > &prjs);

    QTreeView *_view;
    QStandardItemModel *_model;
    QModelIndex _currentIndex;
};

#endif // PROJECTSWIDGET_H
