#include "resultinginternalmetrics.h"

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
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

ResultingInternalMetricsWidget::ResultingInternalMetricsWidget (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        setWindowTitle (trUtf8 ("Результирующий график внутренних метрик"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        {
            QBarSeries *series = new QBarSeries ();

            QValueAxis *axisY = new QValueAxis ();
            axisY->setRange (0, 1);
            //axisY->setTickCount (20);
            //axisY->setMin (0);
            //axisY->setMax (1);
            axisY->applyNiceNumbers ();
            series->attachAxis (axisY);

            //        QLineSeries *lineseries = new QLineSeries();
            //        lineseries->setName ("max");
            //        lineseries->append (QPoint (1, 0));
            //        lineseries->append (QPoint (1, 1));

            //-- Fault removal
//            {
//                int A = RedmineInstance::instance ().metric4A (prjid);
//                int B = RedmineInstance::instance ().metric4B (prjid);
//                float X = 0;
//                if (B != 0)
//                    X = (float)A/(float)B;
//                else
//                    X = 1;

//                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Устранение ошибок - %1</b></font>").arg (QString::number (X, 'f', 2)));
//                *set << X;
//                series->append (set);
//            }

            //-- Test adequance
            {
                int A = RedmineInstance::instance ().metric5A (prjid);
                int B = RedmineInstance::instance ().metric5B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Адекватность теста  - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            //-- Failure avoidance
            {
                int A = RedmineInstance::instance ().metric2A (prjid);
                int B = RedmineInstance::instance ().metric2B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Коэффициент отказов - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            //-- Fault detection

            {
                int A = RedmineInstance::instance ().metric3A (prjid);
                int B = RedmineInstance::instance ().metric3B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Обнаружение ошибок - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            //-- Incorrect operation avoidance
            {
                int A = RedmineInstance::instance ().metric1A (prjid);
                int B = RedmineInstance::instance ().metric1B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Способность к предотвращению некорректных действий - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            //-- Restorability
            {
                int A = RedmineInstance::instance ().metric6A (prjid);
                int B = RedmineInstance::instance ().metric6B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Восстанавливаемость - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            //-- Reliability Compliance
            {
                int A = RedmineInstance::instance ().metric7A (prjid);
                int B = RedmineInstance::instance ().metric7B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Соответствие надежности - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->addAxis (axisY, Qt::AlignLeft);
            chart->setTitle (trUtf8 ("<font size=18><b>Результирующий график внутренних метрик</b></font>"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}
