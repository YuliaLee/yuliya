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
#include "metrics/maturitymetrics.h"
#include "metrics/faulttolerancemetrics.h"
#include "metrics/recoverabilitymetrics.h"
#include "metrics/reliabilitycompliancemetrics.h"
#include "metrics/resultinginternalmetrics.h"
#include "metrics/resultingexternalmetrics.h"

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
    _resultingInternalMetricsAct = new QAction (QIcon(":/images/sum.png"), tr ("Resulting Internal Metrics"), this);
    _resultingInternalMetricsAct->setToolTip (tr ("Resulting Internal Metrics"));
    connect (_resultingInternalMetricsAct, SIGNAL(triggered()), this, SLOT(resultingInternalMetrics()));

    _maturityMetricsAct = new QAction (QIcon(":/images/chart.png"), tr ("Maturity Metrics"), this);
    _maturityMetricsAct->setToolTip (tr ("Maturity Metrics"));
    connect (_maturityMetricsAct, SIGNAL(triggered()), this, SLOT(maturityMetrics()));

    _faultToleranceAct = new QAction (QIcon(":/images/chart.png"), tr ("Fault Tolerance Metrics"), this);
    _faultToleranceAct->setToolTip (tr ("Fault Tolerance Metrics"));
    connect (_faultToleranceAct, SIGNAL(triggered()), this, SLOT(faultToleranceMetrics()));

    _recoverabilityAct = new QAction (QIcon(":/images/chart.png"), tr ("Recoverability Meterics"), this);
    _recoverabilityAct->setToolTip (tr ("Recoverability Metrics"));
    connect (_recoverabilityAct, SIGNAL(triggered()), this, SLOT(recoverabilityMetrics()));

    _reliabilityComplianceAct = new QAction (QIcon(":/images/chart.png"), tr ("Reliability Compliance Metrics"), this);
    _reliabilityComplianceAct->setToolTip (tr ("Reliability Compliance Metrics"));
    connect (_reliabilityComplianceAct, SIGNAL(triggered()), this, SLOT(reliabilityComplianceMetrics()));

    //---------------------------------------------------

    _resultingExternalMetricsAct = new QAction (QIcon(":/images/sum.png"), tr ("Resulting External Metrics"), this);
    _resultingExternalMetricsAct->setToolTip (tr ("Resulting External Metrics"));
    connect (_resultingExternalMetricsAct, SIGNAL(triggered()), this, SLOT(resultingExternalMetrics()));
}

void MainWindow::createToolBars ()
{
    _internalToolBar = addToolBar (tr ("Internal metrics"));
    _internalToolBar->addWidget (new QLabel (tr ("Internal metrics")));
    _internalToolBar->addAction (_resultingInternalMetricsAct);
    _internalToolBar->addAction (_maturityMetricsAct);
    _internalToolBar->addAction (_faultToleranceAct);
    _internalToolBar->addAction (_recoverabilityAct);
    _internalToolBar->addAction (_reliabilityComplianceAct);

    _externalToolBar = addToolBar (tr ("External"));
    _externalToolBar->addWidget (new QLabel (tr ("External metrics")));
    _externalToolBar->addAction (_resultingExternalMetricsAct);
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

void MainWindow::maturityMetrics ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    MaturityMetricsWidget *w = new MaturityMetricsWidget (prjid, _mdiArea);
    QMdiSubWindow *child = _mdiArea->addSubWindow (w);
    child->resize (800, 600);
    child->show ();
}

void MainWindow::faultToleranceMetrics ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    FaultToleranceMetrics *w = new FaultToleranceMetrics (prjid, _mdiArea);
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
