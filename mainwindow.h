#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "projectswidget.h"

class MdiChild;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow ();

protected:
    void closeEvent (QCloseEvent *event);
    void showEvent (QShowEvent *ev);

private slots:
    void updateToolbar (const QString &prjid);

    //! Инициализация соединений с БД
    void initConnections ();

    //! Инициализация панли со списокм проектов
    void initProjectList ();

    void maturityMetrics ();
    void faultToleranceMetrics ();
    void recoverabilityMetrics ();
    void reliabilityComplianceMetrics ();
    void resultingInternalMetrics ();

    void resultingExternalMetrics ();

private:
    void createActions ();
    void createToolBars ();
    void createStatusBar ();
    void readSettings ();
    void writeSettings ();

    QMdiArea *_mdiArea;

    QToolBar *_internalToolBar;
    QAction *_maturityMetricsAct;
    QAction *_faultToleranceAct;
    QAction *_recoverabilityAct;
    QAction *_reliabilityComplianceAct;
    QAction *_resultingInternalMetricsAct;

    QToolBar *_externalToolBar;
    QAction *_resultingExternalMetricsAct;

    ProjectsWidget *_projectsWidget;
};

#endif
