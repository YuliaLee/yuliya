#include "maturitymetrics.h"

#include "redmineinstance.h"
#include "charts/drilldownchart.h"
#include "charts/drilldownslice.h"

#include <QDebug>
#include <QHBoxLayout>

#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QPieSeries>

QT_CHARTS_USE_NAMESPACE

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

        //-- обнаружение ошибок
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число обнаруженных ошибок
            series->append (trUtf8 ("Число обнаруженных ошибок"), 1);

            int A = 0;
            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            for (int i = 0; i < project->_issues.size (); ++i)
            {
                //-- ищем все ошибки
                if (project->_issues[i]->_tracker_id == "1")
                    A++;
            }
            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setLabel (trUtf8 ("Число обнаруженных ошибок - %1").arg (QString::number (A)));

            //-------------- Планируемое число ошибок

            series->append (trUtf8 ("Планируемое число ошибок"), 2);
            int B = project->_reference_number_of_error;
            slice = series->slices ().at (1);
            slice->setValue (B);
            slice->setLabel (trUtf8 ("Планируемое число ошибок - %1").arg (QString::number (B)));

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Обнаружение ошибок"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            //layout ()->addWidget (chartView);
            hl->addWidget (chartView);
        }

        //-- устранение ошибок
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число решённых ошибок
            series->append (trUtf8 ("Число решённых ошибок"), 1);

            int A = 0;
            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            for (int i = 0; i < project->_issues.size (); ++i)
            {
                //-- ищем закрытые ошибки
                if (project->_issues[i]->_tracker_id == "1" &&
                        project->_issues[i]->_status_id == "3")
                    A++;
            }
            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setLabel (trUtf8 ("Число решённых ошибок - %1").arg (QString::number (A)));

            //-------------- Число не решённых ошибок
            series->append (trUtf8 ("Число не решённых ошибок"), 2);

            int B = 0;
            for (int i = 0; i < project->_issues.size (); ++i)
            {
                //-- ищем все ошибки
                if (project->_issues[i]->_tracker_id == "1")
                    B++;
            }

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Число не решённых ошибок - %1").arg (QString::number (B - A)));

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Устранение ошибок"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            //layout ()->addWidget (chartView);
            hl->addWidget (chartView);
        }

        //-- адекватность теста
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Подтверждённое кол-во тест-кейсов
            series->append (trUtf8 ("Подтверждённое кол-во тест-кейсов:"), 1);

            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            int A = project->_created_test_case;

            QPieSlice *slice = series->slices ().at (0);
            slice->setLabel (trUtf8 ("Подтверждённое кол-во тест-кейсов - %1").arg (QString::number (A)));
            slice->setValue (A);

            //-------------- Не подтверждённое кол-во тест-кейсов
            series->append (trUtf8 ("Не подтверждённое кол-во тест-кейсов:"), 2);

            int B = project->_need_test_case;

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не подтверждённое кол-во тест-кейсов - %1").arg (QString::number (B - A)));

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Адекватность теста"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            //layout ()->addWidget (chartView);
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
