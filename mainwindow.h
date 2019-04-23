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
    void newFile ();
    void open ();
    void save ();
    void saveAs ();
    void cut ();
    void copy ();
    void paste ();
    void about ();
    void updateMenus ();
    void updateWindowMenu ();
    MdiChild *createMdiChild ();
    void setActiveSubWindow (QWidget *window);

    //! Инициализация соединений с БД
    void initConnections ();

    //! Инициализация панли со списокм проектов
    void initProjectList ();

    void maturityMetrics ();
    void faultToleranceMetrics ();
    void recoverabilityMetrics ();
    void reliabilityComplianceMetrics ();
    void resultingChartofMetrics();

private:

    void createActions ();
    void createMenus ();
    void createToolBars ();
    void createStatusBar ();
    void readSettings ();
    void writeSettings ();

    MdiChild *activeMdiChild ();
    QMdiSubWindow *findMdiChild (const QString &fileName);

    QMdiArea *_mdiArea;
    QSignalMapper *_windowMapper;

    QMenu *_fileMenu;
    QMenu *_editMenu;
    QMenu *_windowMenu;
    QMenu *_helpMenu;
    QToolBar *_fileToolBar;
    QToolBar *_editToolBar;
    QAction *_newAct;
    QAction *_openAct;
    QAction *_saveAct;
    QAction *_saveAsAct;
    QAction *_exitAct;
    QAction *_cutAct;
    QAction *_copyAct;
    QAction *_pasteAct;
    QAction *_closeAct;
    QAction *_closeAllAct;
    QAction *_tileAct;
    QAction *_cascadeAct;
    QAction *_nextAct;
    QAction *_previousAct;
    QAction *_separatorAct;
    QAction *_aboutAct;
    QAction *_aboutQtAct;

    QToolBar *_internalToolBar;
    QAction *_maturityMetricsAct;
    QAction *_faultToleranceAct;
    QAction *_recoverabilityAct;
    QAction *_reliabilityComplianceAct;
    QAction *_resultingChartofMetricsAct;
    QToolBar *_externalToolBar;

    ProjectsWidget *_projectsWidget;
};

#endif
