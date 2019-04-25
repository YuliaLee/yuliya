#include "internalfaulttolerancemetrics.h"

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

FaultToleranceMetrics::FaultToleranceMetrics (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внутренние метрики устойчивости к ошибкам"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        //-- Способность к предотвращению некоррестных действий
        {
            int A = RedmineInstance::instance ().metric1A (prjid);
            QBarSet *set0 = new QBarSet (trUtf8 ("Количество разработанных задач для предотвращения некорректных действий - %1").arg (QString::number (A)));
            *set0 << A;

            int B = RedmineInstance::instance ().metric1B (prjid);
            QBarSet *set1 = new QBarSet (trUtf8 ("Планируемое количество некорректных действий - %1").arg (QString::number (B)));
            *set1 << B;

            QBarSeries *series = new QBarSeries ();
            series->append (set0);
            series->append (set1);

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Способность к предотвращению некоррестных действий"));
            chart->setAnimationOptions (QChart::SeriesAnimations);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }

        //-- Коэффициент отказов
        {
            QPieSeries *series = new QPieSeries ();

            //-------------- Число предотвращённых исключений
            series->append (trUtf8 ("Число предотвращённых исключений"), 1);

            int A = RedmineInstance::instance ().metric2A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setLabel (trUtf8 ("Число предотвращённых исключений - %1").arg (QString::number (A)));

            //-------------- Число НЕ предотвращённых исключений
            series->append (trUtf8 ("Число НЕ предотвращённых исключений"), 2);

            int B = RedmineInstance::instance ().metric2B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Число НЕ предотвращённых исключений - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (4);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Коэффициент отказов"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}

FaultToleranceMetrics::~FaultToleranceMetrics ()
{
}
