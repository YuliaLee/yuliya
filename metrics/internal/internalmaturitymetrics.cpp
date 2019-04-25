#include "internalmaturitymetrics.h"

#include "redmineinstance.h"
#include "charts/drilldownchart.h"
#include "charts/drilldownslice.h"

#include <QDebug>
#include <QHBoxLayout>

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QPieSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

using namespace Internal;

MaturityMetricsWidget::MaturityMetricsWidget (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внутренние метрики завершённости"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        QHBoxLayout *hl = new QHBoxLayout ();
        hl->setContentsMargins (4,4,4,4);
        vl->addLayout (hl);

        //-- Обнаружение ошибок
        {
            int A = RedmineInstance::instance ().metric3A (prjid);
            //            int A = 0;
            //            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            //            for (int i = 0; i < project->_issues.size (); ++i)
            //            {
            //                //-- ищем все ошибки
            //                if (project->_issues[i]->_tracker_id == "1")
            //                    A++;
            //            }
            QBarSet *set0 = new QBarSet (trUtf8 ("Число обнаруженных ошибок - %1").arg (QString::number (A)));
            *set0 << A;

            int B = RedmineInstance::instance ().metric3B (prjid);
            //            int B = project->_reference_number_of_error;
            QBarSet *set1 = new QBarSet (trUtf8 ("Планируемое число ошибок - %1").arg (QString::number (B)));
            *set1 << B;

            QBarSeries *series = new QBarSeries ();
            series->append (set0);
            series->append (set1);

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Обнаружение ошибок"));
            chart->setAnimationOptions (QChart::SeriesAnimations);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            hl->addWidget (chartView);
        }

        //-- Устранение ошибок
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число решённых ошибок
            series->append (trUtf8 ("Число решённых ошибок"), 1);

            int A = RedmineInstance::instance ().metric4A (prjid);
            //            int A = 0;
            //            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            //            for (int i = 0; i < project->_issues.size (); ++i)
            //            {
            //                //-- ищем закрытые ошибки
            //                if (project->_issues[i]->_tracker_id == "1" &&
            //                        project->_issues[i]->_status_id == "3")
            //                    A++;
            //            }
            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setLabel (trUtf8 ("Число решённых ошибок - %1").arg (QString::number (A)));

            //-------------- Число не решённых ошибок
            series->append (trUtf8 ("Число не решённых ошибок"), 2);

            int B = RedmineInstance::instance ().metric4B (prjid);
            //            int B = 0;
            //            for (int i = 0; i < project->_issues.size (); ++i)
            //            {
            //                //-- ищем все ошибки
            //                if (project->_issues[i]->_tracker_id == "1")
            //                    B++;
            //            }

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Число не решённых ошибок - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (4);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Устранение ошибок"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            hl->addWidget (chartView);
        }

        //-- Адекватность теста
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Подтверждённое кол-во тест-кейсов
            series->append (trUtf8 ("Подтверждённое кол-во тест-кейсов:"), 1);

            //QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            //int A = project->_created_test_case;
            int A = RedmineInstance::instance ().metric5A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setLabel (trUtf8 ("Подтверждённое кол-во тест-кейсов - %1").arg (QString::number (A)));
            slice->setValue (A);

            //-------------- Не подтверждённое кол-во тест-кейсов
            series->append (trUtf8 ("Не подтверждённое кол-во тест-кейсов:"), 2);

            //int B = project->_need_test_case;
            int B = RedmineInstance::instance ().metric5B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не подтверждённое кол-во тест-кейсов - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (4);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Адекватность теста"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);

            vl->addWidget (chartView);
        }
    }
    else
        qCritical () << "[OneChartWidget][OneChartWidget] Could not find project"
                     << prjid;
}

MaturityMetricsWidget::~MaturityMetricsWidget ()
{
}
