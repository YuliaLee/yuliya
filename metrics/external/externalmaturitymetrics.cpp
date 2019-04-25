#include "externalmaturitymetrics.h"

#include "redmineinstance.h"
#include "charts/drilldownchart.h"
#include "charts/drilldownslice.h"

#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>

#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QPieSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

using namespace External;

MaturityMetricsWidget1::MaturityMetricsWidget1 (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внешние метрики завершённости"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        //        QHBoxLayout *hl = new QHBoxLayout ();
        //        hl->setContentsMargins (4,4,4,4);
        //        vl->addLayout (hl);

        {
            QLineSeries *series = new QLineSeries ();
            QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
            for (int i = 0; i < metrics.size (); ++i)
            {
                float X = (float)abs (RedmineInstance::instance ().projects ()[prjid]->_reference_number_of_error
                                      - metrics[i]._errors.toInt ()) / (float)metrics[i]._code_lines.toInt ();
                series->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
            }

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (tr ("Estimated latent fault density"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }

        {
            QLineSeries *series = new QLineSeries ();
            QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
            for (int i = 0; i < metrics.size (); ++i)
            {
                float X = metrics[i]._errors.toFloat () / metrics[i]._test_cases.toFloat ();
                series->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
            }

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (tr ("Failure density against test cases"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }

        {
            QLineSeries *series = new QLineSeries ();
            QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
            for (int i = 0; i < metrics.size (); ++i)
            {
                float X = (float)metrics[i]._errors.toFloat () / metrics[i]._code_lines.toFloat ();
                series->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
            }

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (tr ("Fault density"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
    else
        qCritical () << "[MaturityMetrics][MaturityMetrics] Could not find project"
                     << prjid;
}

MaturityMetricsWidget2::MaturityMetricsWidget2 (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внешние метрики завершённости"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        {
            float X = (float)RedmineInstance::instance ().metric4A (prjid) /
                    (float)RedmineInstance::instance ().metric4B (prjid);
            QBarSet *set0 = new QBarSet (trUtf8 ("Кол-во исправленных ошибок - %1").arg (QString::number (X)));
            *set0 << X;

            float Y = (float)RedmineInstance::instance ().metric4A (prjid) /
                    (float)RedmineInstance::instance ().metric3B (prjid);
            QBarSet *set1 = new QBarSet (trUtf8 ("Y - %1").arg (QString::number (Y)));
            *set1 << Y;

            QBarSeries *series = new QBarSeries ();
            series->append (set0);
            series->append (set1);

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Устранение ошибок"));
            chart->setAnimationOptions (QChart::SeriesAnimations);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}
