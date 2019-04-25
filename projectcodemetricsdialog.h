#ifndef PROJECTCODEMETRICSDIALOG_H
#define PROJECTCODEMETRICSDIALOG_H

#include <QToolBar>
#include <QAction>
#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>

class ProjectCodeMetricsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectCodeMetricsDialog (const QString &prjid, QWidget *parent = nullptr);
    ~ProjectCodeMetricsDialog ();

protected slots:
    void slotSelectionChanged (const QItemSelection &selected, const QItemSelection &deselected);
    void slotAdd ();
    void slotDelete ();

protected:
    void writeSettings ();
    void readSettings ();

    QToolBar *_toolBar;
    QAction *_addAct;
    QAction *_delAct;

    QTableView *_view;
    QStandardItemModel *_model;
    QModelIndex _index;

    QString _prjid;
};

#endif //PROJECTCODEMETRICSDIALOG_H
