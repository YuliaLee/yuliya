#include "resultingexternalmetrics.h"

#include "redmineinstance.h"
#include "charts/drilldownchart.h"
#include "charts/drilldownslice.h"

#include <QDateTime>
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
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

ResultingExternalMetricsWidget::ResultingExternalMetricsWidget(const QString &prjid, QWidget *parent)
    : QWidget(parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        setWindowTitle (trUtf8 ("Результирующие диаграммы внешних метрик"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        {
            QLineSeries *series1 = new QLineSeries ();
            {
                series1->setName (trUtf8 ("Оцениваемая плотность скрытых ошибок"));
                QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
                for (int i = 0; i < metrics.size (); ++i)
                {
                    float X = (float)abs (RedmineInstance::instance ().projects ()[prjid]->_reference_number_of_error
                                          - metrics[i]._errors.toInt ()) / (float)metrics[i]._code_lines.toInt ();
                    series1->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
                }
            }

            QLineSeries *series2 = new QLineSeries ();
            {
                series2->setName (trUtf8 ("Плотность отказов по отношению к тест-кейсам"));
                QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
                for (int i = 0; i < metrics.size (); ++i)
                {
                    float X = metrics[i]._errors.toFloat () / metrics[i]._test_cases.toFloat ();
                    series2->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
                }
            }

            QLineSeries *series3 = new QLineSeries ();
            {
                series3->setName (trUtf8 ("Плотность ошибок"));
                QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
                for (int i = 0; i < metrics.size (); ++i)
                {
                    float X = (float)metrics[i]._errors.toFloat () / metrics[i]._code_lines.toFloat ();
                    series3->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
                    //qDebug () << (float)metrics[i]._errors.toFloat () << metrics[i]._code_lines.toFloat () << X;
                }
            }

            QChart *chart = new QChart ();
            chart->addSeries (series1);
            chart->addSeries (series2);
            chart->addSeries (series3);
            //chart->setTitle (trUtf8 ("Оцениваемая плотность скрытых ошибок"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}

ResultingExternalMetricsWidget2::ResultingExternalMetricsWidget2(const QString &prjid, QWidget *parent)
    : QWidget(parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        setWindowTitle (trUtf8 ("Результирующие диаграммы внешних метрик"));
        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        {
            QBarSeries *series = new QBarSeries ();

            QValueAxis *axisY = new QValueAxis ();
            axisY->setRange (0, 1);
            axisY->applyNiceNumbers ();
            series->attachAxis (axisY);

        //-- Fault removal
            {
                    int A = RedmineInstance::instance ().metric4A (prjid);
                    int B = RedmineInstance::instance ().metric4B (prjid);
                    float X = 0;
                    if (B != 0)
                        X = (float)A/(float)B;
                    else
                       X = 1;

                    QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Устранение ошибок - %1</b></font>").arg (QString::number (X, 'f', 2)));
                    *set << X;
                    series->append (set);

            }

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->addAxis (axisY, Qt::AlignLeft);
            chart->setTitle (trUtf8 ("<font size=18><b>Результирующиt график внутренних метрик</b></font>"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
       }
    }
}
