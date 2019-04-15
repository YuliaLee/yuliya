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

#include "connectiondialog.h"
#include "projectswidget.h"
#include "redmineinstance.h"
#include "onechartwidget.h"

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
    connect (_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateMenus()));
    _windowMapper = new QSignalMapper (this);
    connect (_windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));

    createActions ();
    createMenus ();
    createToolBars ();
    createStatusBar ();
    updateMenus ();

    readSettings ();

    setWindowTitle (tr ("MDI"));
    setUnifiedTitleAndToolBarOnMac (true);
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

void MainWindow::newFile()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->show();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) {
            _mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        } else {
            child->close();
        }
    }
}

void MainWindow::save()
{
    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::saveAs()
{
    if (activeMdiChild() && activeMdiChild()->saveAs())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::cut()
{
    if (activeMdiChild())
        activeMdiChild()->cut();
}

void MainWindow::copy()
{
    if (activeMdiChild())
        activeMdiChild()->copy();
}

void MainWindow::paste()
{
    if (activeMdiChild())
        activeMdiChild()->paste();
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About MDI"),
            tr("The <b>MDI</b> example demonstrates how to write multiple "
               "document interface applications using Qt."));
}

void MainWindow::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != 0);
    _saveAct->setEnabled(hasMdiChild);
    _saveAsAct->setEnabled(hasMdiChild);
    _pasteAct->setEnabled(hasMdiChild);
    _closeAct->setEnabled(hasMdiChild);
    _closeAllAct->setEnabled(hasMdiChild);
    _tileAct->setEnabled(hasMdiChild);
    _cascadeAct->setEnabled(hasMdiChild);
    _nextAct->setEnabled(hasMdiChild);
    _previousAct->setEnabled(hasMdiChild);
    _separatorAct->setVisible(hasMdiChild);

    bool hasSelection = (activeMdiChild() &&
                         activeMdiChild()->textCursor().hasSelection());
    _cutAct->setEnabled(hasSelection);
    _copyAct->setEnabled(hasSelection);
}

void MainWindow::updateWindowMenu()
{
    _windowMenu->clear();
    _windowMenu->addAction(_closeAct);
    _windowMenu->addAction(_closeAllAct);
    _windowMenu->addSeparator();
    _windowMenu->addAction(_tileAct);
    _windowMenu->addAction(_cascadeAct);
    _windowMenu->addSeparator();
    _windowMenu->addAction(_nextAct);
    _windowMenu->addAction(_previousAct);
    _windowMenu->addAction(_separatorAct);

    QList<QMdiSubWindow *> windows = _mdiArea->subWindowList();
    _separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        MdiChild *child = qobject_cast<MdiChild *>(windows.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1)
                               .arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1)
                              .arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = _windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), _windowMapper, SLOT(map()));
        _windowMapper->setMapping(action, windows.at(i));
    }
}

MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    _mdiArea->addSubWindow(child);

    connect(child, SIGNAL(copyAvailable(bool)),
            _cutAct, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)),
            _copyAct, SLOT(setEnabled(bool)));

    return child;
}

void MainWindow::createActions()
{
    _newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    _newAct->setShortcuts(QKeySequence::New);
    _newAct->setStatusTip(tr("Create a new file"));
    connect(_newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    _openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    _openAct->setShortcuts(QKeySequence::Open);
    _openAct->setStatusTip(tr("Open an existing file"));
    connect(_openAct, SIGNAL(triggered()), this, SLOT(open()));

    _saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    _saveAct->setShortcuts(QKeySequence::Save);
    _saveAct->setStatusTip(tr("Save the document to disk"));
    connect(_saveAct, SIGNAL(triggered()), this, SLOT(save()));

    _saveAsAct = new QAction(tr("Save &As..."), this);
    _saveAsAct->setShortcuts(QKeySequence::SaveAs);
    _saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(_saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

//! [0]
    _exitAct = new QAction(tr("E&xit"), this);
    _exitAct->setShortcuts(QKeySequence::Quit);
    _exitAct->setStatusTip(tr("Exit the application"));
    connect(_exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
//! [0]

    _cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    _cutAct->setShortcuts(QKeySequence::Cut);
    _cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(_cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    _copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    _copyAct->setShortcuts(QKeySequence::Copy);
    _copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(_copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    _pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    _pasteAct->setShortcuts(QKeySequence::Paste);
    _pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(_pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    _closeAct = new QAction(tr("Cl&ose"), this);
    _closeAct->setStatusTip(tr("Close the active window"));
    connect(_closeAct, SIGNAL(triggered()),
            _mdiArea, SLOT(closeActiveSubWindow()));

    _closeAllAct = new QAction(tr("Close &All"), this);
    _closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(_closeAllAct, SIGNAL(triggered()),
            _mdiArea, SLOT(closeAllSubWindows()));

    _tileAct = new QAction(tr("&Tile"), this);
    _tileAct->setStatusTip(tr("Tile the windows"));
    connect(_tileAct, SIGNAL(triggered()), _mdiArea, SLOT(tileSubWindows()));

    _cascadeAct = new QAction(tr("&Cascade"), this);
    _cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(_cascadeAct, SIGNAL(triggered()), _mdiArea, SLOT(cascadeSubWindows()));

    _nextAct = new QAction(tr("Ne&xt"), this);
    _nextAct->setShortcuts(QKeySequence::NextChild);
    _nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(_nextAct, SIGNAL(triggered()),
            _mdiArea, SLOT(activateNextSubWindow()));

    _previousAct = new QAction(tr("Pre&vious"), this);
    _previousAct->setShortcuts(QKeySequence::PreviousChild);
    _previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(_previousAct, SIGNAL(triggered()),
            _mdiArea, SLOT(activatePreviousSubWindow()));

    _separatorAct = new QAction(this);
    _separatorAct->setSeparator(true);

    _aboutAct = new QAction(tr("&About"), this);
    _aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(_aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    _aboutQtAct = new QAction(tr("About &Qt"), this);
    _aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(_aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    //---------------------------------------------------

    _oneAct = new QAction (QIcon(":/images/chart.png"), tr ("One"), this);
    connect (_oneAct, SIGNAL(triggered()), this, SLOT(one()));
}

void MainWindow::createMenus()
{
    _fileMenu = menuBar()->addMenu(tr("&File"));
    _fileMenu->addAction(_newAct);
    _fileMenu->addAction(_openAct);
    _fileMenu->addAction(_saveAct);
    _fileMenu->addAction(_saveAsAct);
    _fileMenu->addSeparator();
    _fileMenu->addAction(_exitAct);

    _editMenu = menuBar()->addMenu(tr("&Edit"));
    _editMenu->addAction(_cutAct);
    _editMenu->addAction(_copyAct);
    _editMenu->addAction(_pasteAct);

    _windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(_windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    _helpMenu = menuBar()->addMenu(tr("&Help"));
    _helpMenu->addAction(_aboutAct);
    _helpMenu->addAction(_aboutQtAct);
}

void MainWindow::createToolBars()
{
    _fileToolBar = addToolBar(tr("File"));
    _fileToolBar->addAction(_newAct);
    _fileToolBar->addAction(_openAct);
    _fileToolBar->addAction(_saveAct);

    _editToolBar = addToolBar(tr("Edit"));
    _editToolBar->addAction(_cutAct);
    _editToolBar->addAction(_copyAct);
    _editToolBar->addAction(_pasteAct);

    _internalToolBar = addToolBar (tr ("Internal"));
    _internalToolBar->addAction (_oneAct);
    _externalToolBar = addToolBar (tr ("External"));
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings settings("Trolltech", "MDI Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings("Trolltech", "MDI Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

MdiChild *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = _mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, _mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}

void MainWindow::setActiveSubWindow (QWidget *window)
{
    if (!window)
        return;
    _mdiArea->setActiveSubWindow (qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::initConnections ()
{
    ConnectionDialog dlg (this);
    int ret = dlg.exec ();
    if (ret == QDialog::Accepted) {
        initProjectList ();
    }
}

void MainWindow::initProjectList ()
{
    _projectsWidget = new ProjectsWidget (nullptr);
    QDockWidget *doc = new QDockWidget ();
    doc->setWidget (_projectsWidget);
    addDockWidget (Qt::LeftDockWidgetArea, doc);
}

void MainWindow::one ()
{
    QString prjid = _projectsWidget->selectedProject ();
    if (prjid.isEmpty ())
        return;

    OneChartWidget *w = new OneChartWidget (prjid, _mdiArea);
    QMdiSubWindow *child = _mdiArea->addSubWindow (w);
    child->resize (800, 600);
    child->show ();
}
