#include "resultingchartofmetrics.h"

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

ResultingInternalMetricsWidget::ResultingInternalMetricsWidget (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Resulting Charts of Internal Metrics"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        {
//            int A = RedmineInstance::instance ().metric1A (prjid);

//            QBarSet *set0 = new QBarSet (trUtf8 ("Количество разработанных задач для предотвращения некорректных действий - %1").arg (QString::number (A)));
//            *set0 << A;

//            //int B = project->_incorrect_actions;
//            int B = RedmineInstance::instance ().metric1B (prjid);
//            QBarSet *set1 = new QBarSet (trUtf8 ("Планируемое количество некорректных действий - %1").arg (QString::number (B)));
//            *set1 << B;

//            QBarSeries *series = new QBarSeries ();
//            series->append (set0);
//            series->append (set1);

//            QChart *chart = new QChart ();
//            chart->addSeries (series);
//            chart->setTitle (trUtf8 ("Способность к предотвращению некоррестных действий"));
//            chart->setAnimationOptions (QChart::SeriesAnimations);

//            chart->legend ()->setVisible (true);
//            chart->legend ()->setAlignment (Qt::AlignBottom);

//            QChartView *chartView = new QChartView(chart);
//            chartView->setRenderHint(QPainter::Antialiasing);
//            vl->addWidget (chartView);
        }
    }
}
