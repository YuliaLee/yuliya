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
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        setWindowTitle (trUtf8 ("Внутренние метрики устойчивости к ошибкам"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        QHBoxLayout *hl = new QHBoxLayout ();
        hl->setContentsMargins (4,4,4,4);
        vl->addLayout (hl);

        //-- Способность к предотвращению некоррестных действий
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Количество предотвращенных некорректных действий
            series->append (trUtf8 ("Предотвращенные некорректные действия"), 1);

            int A = RedmineInstance::instance ().metric1A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(255, 160, 122));
            slice->setLabel (trUtf8 (("Предотвращенные некорректные действия - %1")).arg (QString::number (A)));

            //-------------- Количество Не разработанных задач для предотвращения некорректных действий
            series->append (trUtf8 ("Не предотвращенные некорректные действия"), 2);

            int B = RedmineInstance::instance ().metric1B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не предотвращенные некорректные действия - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(255, 140, 0));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Предотвращение некорректных действий"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }

        //-- Коэффициент отказов
        {
            QPieSeries *series = new QPieSeries ();

            //-------------- Число предотвращённых исключений
            series->append (trUtf8 ("Предотвращённые исключения"), 1);

            int A = RedmineInstance::instance ().metric2A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(124, 252, 0));
            slice->setLabel (trUtf8 ("Предотвращённые исключения - %1").arg (QString::number (A)));

            //-------------- Число НЕ предотвращённых исключений
            series->append (trUtf8 ("Не предотвращённые исключения"), 2);

            int B = RedmineInstance::instance ().metric2B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не предотвращённые исключения - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(154, 205, 50));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Коэффициент отказов"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}

FaultToleranceMetrics::~FaultToleranceMetrics ()
{
}
