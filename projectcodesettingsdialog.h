#ifndef PROJECTCODESETTINGSDIALOG_H
#define PROJECTCODESETTINGSDIALOG_H

#include <QToolBar>
#include <QAction>
#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>

class ProjectCodeSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectCodeSettingsDialog (const QString &prjid, QWidget *parent = nullptr);
    ~ProjectCodeSettingsDialog ();

protected:
    QToolBar *_toolBar;
    QAction *_addAct;
    QAction *_delAct;

    QTableView *_view;
    QStandardItemModel *_model;
};

#endif // PROJECTCODESETTINGSDIALOG_H
