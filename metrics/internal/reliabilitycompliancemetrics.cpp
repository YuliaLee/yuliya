#include "reliabilitycompliancemetrics.h"

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

ReliabilityComplianceMetrics::ReliabilityComplianceMetrics (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        setWindowTitle (trUtf8 ("Внутренняя метрика соответсвия надёжности"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        //-- Внутренняя метрика соответсвия надёжности
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число решённых ошибок
            series->append (trUtf8 ("A"), 1);

            int A = RedmineInstance::instance ().metric7A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(240, 128, 128));
            slice->setLabel (trUtf8 ("<font size=4><b>Соответствующие надежности задачи - %1</b></font>").arg (QString::number (A)));

            //-------------- Число не решённых ошибок
            series->append (trUtf8 ("B"), 2);

            int B = RedmineInstance::instance ().metric7B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("<font size=4><b>Не соответствующие надежности задачи - %1</b></font>").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(178, 34, 34));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("<font size=6><b>Соответсвие надёжности</b></font>"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}
