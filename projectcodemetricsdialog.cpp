#include "projectcodemetricsdialog.h"
#include "projectcodelinesdialog.h"

#include <QSettings>
#include <QDebug>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDateTime>

ProjectCodeMetricsDialog::ProjectCodeMetricsDialog (const QString &prjid, QWidget *parent)
    : QDialog (parent)
    , _toolBar (nullptr)
    , _addAct (nullptr)
    , _delAct (nullptr)
    , _view (nullptr)
    , _model (nullptr)
    , _prjid (prjid)
{
    setWindowTitle (trUtf8 ("Настройки кода"));
    setLayout (new QVBoxLayout);

    _toolBar = new QToolBar;
    _toolBar->setToolButtonStyle (Qt::ToolButtonTextUnderIcon);
    _addAct = _toolBar->addAction (QIcon (":/images/plus-button.png"), trUtf8 ("Добавить"), this, SLOT(slotAdd()));
    _delAct = _toolBar->addAction (QIcon (":/images/minus-button.png"), trUtf8 ("Удалить"), this, SLOT(slotDelete()));
    layout ()->addWidget (_toolBar);

    _model = new QStandardItemModel (0, 5);
    _model->setHorizontalHeaderItem (0, new QStandardItem (trUtf8 ("Дата")));
    _model->setHorizontalHeaderItem (1, new QStandardItem (trUtf8 ("Количество строк кода")));
    _model->setHorizontalHeaderItem (2, new QStandardItem (trUtf8 ("Общее количество обнаруженных ошибок")));
    _model->setHorizontalHeaderItem (3, new QStandardItem (trUtf8 ("Количество тест-кейсов")));
    _model->setHorizontalHeaderItem (4, new QStandardItem (trUtf8 ("Количество новых обнаруженных ошибок")));

    _view = new QTableView;
    _view->setModel (_model);
    _view->setSelectionMode (QAbstractItemView::SingleSelection);
    _view->setSelectionBehavior (QAbstractItemView::SelectRows);
    _view->setEditTriggers (QAbstractItemView::NoEditTriggers);

    _view->verticalHeader()->hide ();
    _view->horizontalHeader ()->setStretchLastSection (true);

    connect (_view->selectionModel (), SIGNAL (selectionChanged (QItemSelection,QItemSelection)),
             SLOT (slotSelectionChanged (QItemSelection,QItemSelection)));
    slotSelectionChanged (QItemSelection(), QItemSelection());

    layout ()->addWidget (_view);
    resize (800, 600);

    readSettings ();
}

ProjectCodeMetricsDialog::~ProjectCodeMetricsDialog ()
{
    if (_model) delete _model;
}

void ProjectCodeMetricsDialog::slotSelectionChanged (const QItemSelection &selected, const QItemSelection &deselected)
{
    (void)deselected;
    if (selected.isEmpty ()) {
        _index = QModelIndex ();
        _delAct->setEnabled (false);
    } else {
        _index = selected.indexes ()[0];
        _delAct->setEnabled (true);
    }
}

void ProjectCodeMetricsDialog::slotAdd ()
{
    ProjectCodeLinesDialog *dlg = new ProjectCodeLinesDialog (_prjid, this);
    int ret = dlg->exec ();
    if (ret == QDialog::Accepted)
    {
        QString code_lines = dlg->property ("code_lines").toString ();
        QString errors = dlg->property ("errors").toString ();
        QString test_cases = dlg->property ("test_cases").toString ();
        QString new_errors = dlg->property ("new_errors").toString ();

        QStandardItem *itemdate = new QStandardItem (QDateTime::currentDateTime ().toString ());
        QStandardItem *itemcodelines = new QStandardItem (code_lines);
        QStandardItem *itemerrors = new QStandardItem (errors);
        QStandardItem *itemtestcases = new QStandardItem (test_cases);
        QStandardItem *itemnewerrors = new QStandardItem (new_errors);

        QList<QStandardItem *> row;
        row << itemdate << itemcodelines << itemerrors << itemtestcases << itemnewerrors;
        _model->appendRow (row);

        writeSettings ();
    }
}

void ProjectCodeMetricsDialog::slotDelete ()
{
    _model->removeRow (_index.row ());

    writeSettings ();
}

void ProjectCodeMetricsDialog::writeSettings ()
{
    QSettings settings ("Inteltech", "yuliya");
    settings.remove (QString ("code_settings_%1").arg (_prjid));

    settings.beginGroup (QString ("code_settings_%1").arg (_prjid));
    for (int i = 0; i < _model->rowCount (); ++i)
    {
        QString value = QString ("%1;%2;%3;%4")
                .arg (_model->item (i, 1)->text ())
                .arg (_model->item (i, 2)->text ())
                .arg (_model->item (i, 3)->text ())
                .arg (_model->item (i, 4)->text ());
        settings.setValue (_model->item (i, 0)->text (), value);
    }
    settings.endGroup ();
}

void ProjectCodeMetricsDialog::readSettings ()
{
    _model->removeRows (0, _model->rowCount ());
    _model->setRowCount (0);

    QSettings settings ("Inteltech", "yuliya");
    settings.beginGroup (QString ("code_settings_%1").arg (_prjid));

    QStringList lst = settings.allKeys ();
    for (int i = 0; i < lst.size (); ++i)
    {
        QStringList values = settings.value (lst[i]).toString ().split (";");

        QStandardItem *itemdate = new QStandardItem (lst[i]);
        QStandardItem *itemcodelines = new QStandardItem (values[0]);
        QStandardItem *itemerrors = new QStandardItem (values[1]);
        QStandardItem *itemtestcases = new QStandardItem (values[2]);
        QStandardItem *itemnewerrors = new QStandardItem (values[3]);

        QList<QStandardItem *> row;
        row << itemdate << itemcodelines << itemerrors << itemtestcases<< itemnewerrors;

        _model->appendRow (row);
    }
    settings.endGroup ();

    _view->resizeColumnsToContents ();
    _view->horizontalHeader ()->setSectionResizeMode (QHeaderView::Stretch);
}
