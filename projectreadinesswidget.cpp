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

    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];

        //-------------------------- ВНУТРЕННИЕ МЕТРИКИ
        {
            QStandardItem *item = new QStandardItem (trUtf8 ("Внутренние метрики"));
            item->setTextAlignment (Qt::AlignCenter);
            QFont f = item->font ();
            f.setBold (true);
            f.setPixelSize (25);
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

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Адекватность теста"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Адекватность теста"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
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

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Коэффициент отказов"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Коэффициент отказов"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
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

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Способность к предотвращению некорректных действий"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Предотвращение некорр. действий"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
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

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Восстанавливаемость"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Восстанавливаемость"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
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

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Соответствие надежности"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Соответствие надежности"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
                itemValue->setIcon (QIcon (":/images/minus.png"));
            else
                itemValue->setIcon (QIcon (":/images/plus.png"));
            _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
        }

        //-------------------------- ВНЕШНИЕ МЕТРИКИ
        {
            QStandardItem *item = new QStandardItem (trUtf8 ("Внешние метрики"));
            item->setTextAlignment (Qt::AlignCenter);
            QFont f = item->font ();
            f.setBold (true);
            f.setPixelSize (25);
            item->setFont (f);
            _model->appendRow (QList<QStandardItem*> () << item << new QStandardItem ());
            ui->_view->setSpan (_model->rowCount () - 1, 0, 1, 2);
        }

        //-- Обнаружение ошибок
        {
            int A = RedmineInstance::instance ().metric3A (prjid);
            int B = RedmineInstance::instance ().metric3B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Обнаружение ошибок"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Обнаружение ошибок"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
                itemValue->setIcon (QIcon (":/images/minus.png"));
            else
                itemValue->setIcon (QIcon (":/images/plus.png"));
            _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
        }

        //-- Устранение ошибок
        {
            int A = RedmineInstance::instance ().metric4A (prjid);
            int B = RedmineInstance::instance ().metric4B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Устранение ошибок"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Устранение ошибок"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
                itemValue->setIcon (QIcon (":/images/minus.png"));
            else
                itemValue->setIcon (QIcon (":/images/plus.png"));
            _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
        }

        //-- Разрешение отказов
        {
            int A = RedmineInstance::instance ().metric9A (prjid);
            int B = RedmineInstance::instance ().metric9B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Разрешение отказов"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Разрешение отказов"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
                itemValue->setIcon (QIcon (":/images/minus.png"));
            else
                itemValue->setIcon (QIcon (":/images/plus.png"));
            _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
        }

        //-- Тестовое покрытие
        {
            int A = RedmineInstance::instance ().metric10A (prjid);
            int B = RedmineInstance::instance ().metric5B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Тестовое покрытие"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Тестовое покрытие"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
                itemValue->setIcon (QIcon (":/images/minus.png"));
            else
                itemValue->setIcon (QIcon (":/images/plus.png"));
            _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
        }

        //-- Завершенность испытаний
        {
            int A = RedmineInstance::instance ().metric11A (prjid);
            int B = RedmineInstance::instance ().metric5B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Завершенность испытаний"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Завершенность испытаний"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
                itemValue->setIcon (QIcon (":/images/minus.png"));
            else
                itemValue->setIcon (QIcon (":/images/plus.png"));
            _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
        }

        //-- Коэффициент аварийных отказов
        {
            int A = RedmineInstance::instance ().metric8A (prjid);
            int B = RedmineInstance::instance ().metric8B (prjid);
            float X = 0;
            if (B != 0)
                X = (1-(float)A/(float)B);
            else
                X = 1;

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Коэффициент аварийных отказов"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Коэффициент аварийных отказов"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
                itemValue->setIcon (QIcon (":/images/minus.png"));
            else
                itemValue->setIcon (QIcon (":/images/plus.png"));
            _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
        }

        //-- Коэффициент отказов
        {
            int A = RedmineInstance::instance ().metric12A (prjid);
            int B = RedmineInstance::instance ().metric12B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Коэффициент отказов"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Коэффициент отказов"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < project->_threshold)
                itemValue->setIcon (QIcon (":/images/minus.png"));
            else
                itemValue->setIcon (QIcon (":/images/plus.png"));
            _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
        }

        //-- Предотвращение некорректных действий
        {
            int A = RedmineInstance::instance ().metric13A (prjid);
            int B = RedmineInstance::instance ().metric13B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            qDebug () << "[ProjectReadinessWidget][ProjectReadinessWidget] Предотвращение некорр. действий"
                      << QString::number (X, 'f', 2);

            QStandardItem *itemName = new QStandardItem (trUtf8 ("Предотвращение некорр. действий"));
            QFont f = itemName->font ();
            f.setPixelSize (18);
            itemName->setFont (f);

            QStandardItem *itemValue = new QStandardItem ();
            if (X < 0.9)
                itemValue->setIcon (QIcon (":/images/minus.png"));
            else
                itemValue->setIcon (QIcon (":/images/plus.png"));
            _model->appendRow (QList<QStandardItem*> () << itemName << itemValue);
        }
    }

    ui->_view->resizeColumnsToContents ();
    ui->_view->horizontalHeader ()->setSectionResizeMode (QHeaderView::Stretch);
}

ProjectReadinessWidget::~ProjectReadinessWidget ()
{
    delete ui;
}
