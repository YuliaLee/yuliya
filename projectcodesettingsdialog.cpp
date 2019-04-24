#include "projectcodesettingsdialog.h"

#include <QVBoxLayout>

ProjectCodeSettingsDialog::ProjectCodeSettingsDialog (const QString &prjid, QWidget *parent)
    : QDialog (parent)
    , _toolBar (nullptr)
    , _addAct (nullptr)
    , _delAct (nullptr)
    , _view (nullptr)
    , _model (nullptr)
{
    setWindowTitle (tr ("Project Code Settings"));
    setLayout (new QVBoxLayout);

    _toolBar = new QToolBar;
    _addAct = _toolBar->addAction (QIcon (), tr ("Add"), this, SLOT(slotAdd()));
    _delAct = _toolBar->addAction (QIcon (), tr ("Delete"), this, SLOT(slotDelete()));
    layout ()->addWidget (_toolBar);

    _view = new QTableView;
    _model = new QStandardItemModel;
    _view->setModel (_model);
    layout ()->addWidget (_view);

    resize (600, 400);
}

ProjectCodeSettingsDialog::~ProjectCodeSettingsDialog ()
{
    if (_model) delete _model;
}
