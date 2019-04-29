#include <QMdiArea>
#include <QTimer>
#include <QSignalMapper>
#include <QCloseEvent>
#include <QFileDialog>
#include <QStatusBar>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMdiSubWindow>
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QSettings>
#include <QDockWidget>
#include <QLabel>

#include "connectiondialog.h"
#include "projectswidget.h"
#include "redmineinstance.h"
#include "metrics/internal/internalmaturitymetrics.h"
#include "metrics/internal/internalfaulttolerancemetrics.h"
#include "metrics/internal/recoverabilitymetrics.h"
#include "metrics/internal/reliabilitycompliancemetrics.h"
#include "metrics/internal/resultinginternalmetrics.h"
#include "metrics/external/resultingexternalmetrics.h"
#include "metrics/external/externalmaturitymetrics.h"
#include "metrics/external/externalfaulttolerancemetrics.h"

#include "mainwindow.h"
#include "mdichild.h"

MainWindow::MainWindow ()
    : _projectsWidget (nullptr)
{
    RedmineInstance::instance ();

    _mdiArea = new QMdiArea;
    _mdiArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    _mdiArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    setCentralWidget (_mdiArea);

    createActions ();
    createToolBars ();
    createStatusBar ();

    readSettings ();

    setWindowTitle (tr ("Metrics"));
    setUnifiedTitleAndToolBarOnMac (true);

    updateToolbar (QString::null);
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    _mdiArea->closeAllSubWindows ();
    if (_mdiArea->currentSubWindow ()) {
        event->ignore ();
    } else {
        RedmineInstance::deleteInstance ();
        writeSettings ();
        event->accept ();
    }
}

void MainWindow::showEvent (QShowEvent *ev)
{
    (void)ev;
    QTimer::singleShot (0, this, SLOT(initConnections()));
}

void MainWindow::updateToolbar (const QString &prjid)
{
    if (!prjid.isEmpty () &&
            RedmineInstance::instance ().projects ().contains (prjid))
    {
        _externalToolBar->setEnabled (true);
        _internalToolBar->setEnabled (true);
    }
    else
    {
        _externalToolBar->setEnabled (false);
        _internalToolBar->setEnabled (false);
    }
}

void MainWindow::createActions ()
{


    //---------------------------------------------------
    _resultingInternalMetricsAct = new QAction (QIcon(":/images/sum.png"), trUtf8 ("Результирующие графики внутренних метрик"), this);
    _resultingInternalMetricsAct->setToolTip (trUtf8 ("Результирующие графики внутренних метрик"));
    connect (_resultingInternalMetricsAct, SIGNAL(triggered()), this, SLOT(resultingInternalMetrics()));

    _internalMaturityMetricsAct = new QAction (QIcon(":/images/chart.png"), trUtf8 ("Внутренние метрики завершенности"), this);
    _internalMaturityMetricsAct->setToolTip (trUtf8 ("Внутренние метрики завершенности"));
    connect (_internalMaturityMetricsAct, SIGNAL(triggered()), this, SLOT(internalMaturityMetrics()));

    _internalFaultToleranceAct = new QAction (QIcon(":/images/chart.png"), trUtf8 ("Внутренние метрики устойчивости к ошибкам"), this);
    _internalFaultToleranceAct->setToolTip (trUtf8 ("Внутренние метрики устойчивости к ошибкам"));
    connect (_internalFaultToleranceAct, SIGNAL(triggered()), this, SLOT(internalFaultToleranceMetrics()));

    _recoverabilityAct = new QAction (QIcon(":/images/chart.png"), trUtf8 ("Внутренние метрики восстанавливаемости"), this);
    _recoverabilityAct->setToolTip (trUtf8 ("Внутренние метрики восстанавливаемости"));
    connect (_recoverabilityAct, SIGNAL(triggered()), this, SLOT(recoverabilityMetrics()));

    _reliabilityComplianceAct = new QAction (QIcon(":/images/chart.png"), trUtf8 ("Внутренняя метрика соответствия надежности"), this);
    _reliabilityComplianceAct->setToolTip (trUtf8 ("RВнутренняя метрика соответствия надежности"));
    connect (_reliabilityComplianceAct, SIGNAL(triggered()), this, SLOT(reliabilityComplianceMetrics()));

    //---------------------------------------------------

    _resultingExternalMetricsAct = new QAction (QIcon(":/images/sum.png"), trUtf8 ("Результирующие графики внешних метрик"), this);
    _resultingExternalMetricsAct->setToolTip (trUtf8 ("Результирующие графики внешних метрик"));
    connect (_resultingExternalMetricsAct, SIGNAL(triggered()), this, SLOT(resultingExternalMetrics()));

    _externalMaturityMetricsAct = new QAction (QIcon(":/images/chart.png"), trUtf8 ("Внешние метрики завершенности"), this);
    _externalMaturityMetricsAct->setToolTip (trUtf8( "Внешние метрики завершенности"));
    connect (_externalMaturityMetricsAct, SIGNAL(triggered()), this, SLOT(externalMaturityMetrics()));

    _externalFaultToleranceAct = new QAction (QIcon(":/images/chart.png"), trUtf8("Внешние метрики устойчивости к ошибкам"), this);
    _externalFaultToleranceAct->setToolTip (trUtf8( "Внешние метрики устойчивости к ошибкам"));
    connect (_externalFaultToleranceAct, SIGNAL(triggered()), this, SLOT(externalFaultToleranceMetrics()));
}

void MainWindow::createToolBars ()
{
    _internalToolBar = addToolBar (trUtf8("Внутренние метрики"));
    _internalToolBar->addWidget (new QLabel (trUtf8("Внутренние метрики")));
    _internalToolBar->addAction (_resultingInternalMetricsAct);
    _internalToolBar->addAction (_internalMaturityMetricsAct);
    _internalToolBar->addAction (_internalFaultToleranceAct);
    _internalToolBar->addAction (_recoverabilityAct);
    _internalToolBar->addAction (_reliabilityComplianceAct);

    _externalToolBar = addToolBar (trUtf8("Внешние метрики"));
    _externalToolBar->addWidget (new QLabel (trUtf8("Внешние метрики")));
    _externalToolBar->addAction (_resultingExternalMetricsAct);
    _externalToolBar->addAction (_externalMaturityMetricsAct);
    _externalToolBar->addAction (_externalFaultToleranceAct);
}

void MainWindow::createStatusBar ()
{
    statusBar ()->showMessage (tr ("Ready"));
}

void MainWindow::readSettings ()
{
    QSettings settings ("Inteltech", "yuliya");
    QPoint pos = settings.value ("pos", QPoint (200, 200)).toPoint();
    QSize size = settings.value ("size", QSize (400, 400)).toSize();
    move (pos);
    resize (size);
}

void MainWindow::writeSettings()
{
    QSettings settings ("Inteltech", "yuliya");
    settings.setValue ("pos", pos ());
    settings.setValue ("size", size ());
}

void MainWindow::initConnections ()
{
    if (_projectsWidget)
        return;

    ConnectionDialog dlg (this);
    int ret = dlg.exec ();
    if (ret == QDialog::Accepted)
        initProjectList ();
}

void MainWindow::initProjectList ()
{
    _projectsWidget = new ProjectsWidget (nullptr);
    QDockWidget *doc = new QDockWidget ();
    doc->setWidget (_projectsWidget);
    addDockWidget (Qt::LeftDockWidgetArea, doc);

    connect (_projectsWidget, SIGNAL(projectSelected(QString)),
             SLOT(updateToolbar(QString)));
}

void MainWindow::internalMaturityMetrics ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    Internal::MaturityMetricsWidget *w = new Internal::MaturityMetricsWidget (prjid, _mdiArea);
    QMdiSubWindow *child = _mdiArea->addSubWindow (w);
    child->resize (800, 600);
    child->show ();
}

void MainWindow::internalFaultToleranceMetrics ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    Internal::FaultToleranceMetrics *w = new Internal::FaultToleranceMetrics (prjid, _mdiArea);
    QMdiSubWindow *child = _mdiArea->addSubWindow (w);
    child->resize (800, 600);
    child->show ();
}

void MainWindow::recoverabilityMetrics ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    RecoverabilityMetrics *w = new RecoverabilityMetrics (prjid, _mdiArea);
    QMdiSubWindow *child = _mdiArea->addSubWindow (w);
    child->resize (800, 600);
    child->show ();
}

void MainWindow::reliabilityComplianceMetrics ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    ReliabilityComplianceMetrics *w = new ReliabilityComplianceMetrics (prjid, _mdiArea);
    QMdiSubWindow *child = _mdiArea->addSubWindow (w);
    child->resize (800, 600);
    child->show ();
}

void MainWindow::resultingInternalMetrics ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    ResultingInternalMetricsWidget *w = new ResultingInternalMetricsWidget (prjid, _mdiArea);
    QMdiSubWindow *child = _mdiArea->addSubWindow (w);
    child->resize (800, 600);
    child->show ();
}

void MainWindow::resultingExternalMetrics ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    ResultingExternalMetricsWidget *w = new ResultingExternalMetricsWidget (prjid, _mdiArea);
    QMdiSubWindow *child = _mdiArea->addSubWindow (w);
    child->resize (800, 600);
    child->show ();
}

void MainWindow::externalMaturityMetrics ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    {
        External::MaturityMetricsWidget1 *w = new External::MaturityMetricsWidget1 (prjid, _mdiArea);
        QMdiSubWindow *child = _mdiArea->addSubWindow (w);
        child->resize (800, 600);
        child->show ();
    }

    {
        External::MaturityMetricsWidget2 *w = new External::MaturityMetricsWidget2 (prjid, _mdiArea);
        QMdiSubWindow *child = _mdiArea->addSubWindow (w);
        child->resize (800, 600);
        child->show ();
    }
}

void MainWindow::externalFaultToleranceMetrics()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    External::FaultToleranceMetrics *w = new External::FaultToleranceMetrics (prjid, _mdiArea);
    QMdiSubWindow *child = _mdiArea->addSubWindow (w);
    child->resize (800, 600);
    child->show ();
}
