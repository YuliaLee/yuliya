#include "projectreadinesswidget.h"
#include "ui_projectreadinesswidget.h"
#include "redmineinstance.h"

#include <QDebug>

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
    ui->_view->horizontalHeader ()->hide ();

    _model = new QStandardItemModel (0, 2);
    ui->_view->setModel (_model);

    {
        QStandardItem *item = new QStandardItem (trUtf8 ("Внутренние метрики"));
        item->setTextAlignment (Qt::AlignCenter);
        QFont f = item->font ();
        f.setBold (true);
        f.setPixelSize (16);
        item->setFont (f);
        _model->appendRow (QList<QStandardItem*> () << item << new QStandardItem ());
        ui->_view->setSpan (0, 0, 1, 2);
    }

    //-- Test adequance - Адекватность теста
    {
        QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
        int A = RedmineInstance::instance ().metric5B (prjid);
        int B = project->_need_test_case;
        float X = 0;
        if (B != 0)
            X = (float)A/(float)B;
        else
            X = 0;

        qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Адекватность теста" << X;

        QStandardItem *itemName = new QStandardItem (trUtf8 ("Адекватность теста"));
        QFont f = itemName->font ();
        f.setPixelSize (14);
        itemName->setFont (f);

        QStandardItem *itemValue = new QStandardItem ();
        if (X < 0.9)
            itemValue->setIcon (QIcon (":/images/minus.png"));
        else
            itemValue->setIcon (QIcon (":/images/plus.png"));
        _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
    }

    //-- Failure avoidance - Коэффициент отказов
    {
        int A = RedmineInstance::instance ().metric2A (prjid);
        int B = RedmineInstance::instance ().metric2B (prjid);
        float X = 0;
        if (B != 0)
            X = (float)A/(float)B;
        else
            X = 1;

        qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Коэффициент отказов" << X;

        QStandardItem *itemName = new QStandardItem (trUtf8 ("Коэффициент отказов"));
        QFont f = itemName->font ();
        f.setPixelSize (14);
        itemName->setFont (f);

        QStandardItem *itemValue = new QStandardItem ();
        if (X < 0.9)
            itemValue->setIcon (QIcon (":/images/minus.png"));
        else
            itemValue->setIcon (QIcon (":/images/plus.png"));
        _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
    }

    //-- Incorrect operation avoidance - Способность к предотвращению некорректных действий
    {
        int A = RedmineInstance::instance ().metric1A (prjid);
        int B = RedmineInstance::instance ().metric1B (prjid);
        float X = 0;
        if (B != 0)
            X = (float)A/(float)B;
        else
            X = 1;

        qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Способность к предотвращению некорректных действий" << X;

        QStandardItem *itemName = new QStandardItem (trUtf8 ("Предотвращение некорр. действий"));
        QFont f = itemName->font ();
        f.setPixelSize (14);
        itemName->setFont (f);

        QStandardItem *itemValue = new QStandardItem ();
        if (X < 0.9)
            itemValue->setIcon (QIcon (":/images/minus.png"));
        else
            itemValue->setIcon (QIcon (":/images/plus.png"));
        _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
    }

    //-- Restorability - Восстанавливаемость
    {
        int A = RedmineInstance::instance ().metric6A (prjid);
        int B = RedmineInstance::instance ().metric6B (prjid);
        float X = 0;
        if (B != 0)
            X = (float)A/(float)B;
        else
            X = 1;

        qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Восстанавливаемость" << X;

        QStandardItem *itemName = new QStandardItem (trUtf8 ("Восстанавливаемость"));
        QFont f = itemName->font ();
        f.setPixelSize (14);
        itemName->setFont (f);

        QStandardItem *itemValue = new QStandardItem ();
        if (X < 0.9)
            itemValue->setIcon (QIcon (":/images/minus.png"));
        else
            itemValue->setIcon (QIcon (":/images/plus.png"));
        _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
    }

    //-- Reliability Compliance - Соответствие надежности
    {
        int A = RedmineInstance::instance ().metric7A (prjid);
        int B = RedmineInstance::instance ().metric7B (prjid);
        float X = 0;
        if (B != 0)
            X = (float)A/(float)B;
        else
            X = 1;

        qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Соответствие надежности" << X;

        QStandardItem *itemName = new QStandardItem (trUtf8 ("Соответствие надежности"));
        QFont f = itemName->font ();
        f.setPixelSize (14);
        itemName->setFont (f);

        QStandardItem *itemValue = new QStandardItem ();
        if (X < 0.9)
            itemValue->setIcon (QIcon (":/images/minus.png"));
        else
            itemValue->setIcon (QIcon (":/images/plus.png"));
        _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
    }

    ui->_view->resizeColumnsToContents ();
    ui->_view->horizontalHeader ()->setSectionResizeMode (QHeaderView::Stretch);
}

ProjectReadinessWidget::~ProjectReadinessWidget ()
{
    delete ui;
}
