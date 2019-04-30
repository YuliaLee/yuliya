#include "recoverabilitymetrics.h"

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

RecoverabilityMetrics::RecoverabilityMetrics (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        setWindowTitle (trUtf8 ("Внутренние метрики восстанавливаемости и соответствия надежности"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        //-- Восстанавливаемость
        {
            QPieSeries *series = new QPieSeries();

            //--------------Число предотвращённых исключений
            series->append (trUtf8 ("Число предотвращённых исключений"), 1);

            int A = RedmineInstance::instance ().metric6A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setLabel (trUtf8 (("Число предотвращённых исключений - %1")).arg (QString::number (A)));

            //-------------- Число Не предотвращённых исключений
            series->append (trUtf8 ("Число Не предотвращённых исключений"), 2);

            int B = RedmineInstance::instance ().metric6B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Число Не предотвращённых исключений - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (4);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Восстанавливаемость"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }

        //-- Внутренняя метрика соответсвия надёжности
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число решённых ошибок
            series->append (trUtf8 ("A"), 1);

            int A = RedmineInstance::instance ().metric7A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setLabel (trUtf8 ("A - %1").arg (QString::number (A)));

            //-------------- Число не решённых ошибок
            series->append (trUtf8 ("B"), 2);

            int B = RedmineInstance::instance ().metric7B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("B - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (4);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Внутренняя метрика соответсвия надёжности"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}

