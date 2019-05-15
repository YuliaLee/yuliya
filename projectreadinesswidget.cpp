#include "projectreadinesswidget.h"
#include "ui_projectreadinesswidget.h"

ProjectReadinessWidget::ProjectReadinessWidget (const QString &prjid, QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::ProjectReadinessWidget)
    , _prjid (prjid)
{
    ui->setupUi (this);

    ui->_view->setEditTriggers (QAbstractItemView::NoEditTriggers);
    ui->_view->setSelectionMode (QAbstractItemView::SingleSelection);
    ui->_view->setSelectionBehavior (QAbstractItemView::SelectRows);
    ui->_view->verticalHeader ()->hide ();

    _model = new QStandardItemModel (0, 4);
    ui->_view->setModel (_model);

    _model->setHorizontalHeaderLabels (QStringList ()
                                       << trUtf8 ("Внутренняя метрика")
                                       << trUtf8 ("-/+")
                                       << trUtf8 ("Внешняя метрика")
                                       << trUtf8 ("-/+"));
}

ProjectReadinessWidget::~ProjectReadinessWidget ()
{
    delete ui;
}
