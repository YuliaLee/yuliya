#include "externalfaulttolerancemetrics.h"

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

using namespace External;

FaultToleranceMetrics::FaultToleranceMetrics (const QString &prjid, QWidget *parent)
    : QWidget(parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внешние метрики устойчивости к ошибкам"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        QHBoxLayout *hl = new QHBoxLayout ();
        hl->setContentsMargins (4,4,4,4);
        vl->addLayout (hl);

           //--------- Коэффициент аварийных отказов
        {
            QPieSeries *series = new QPieSeries ();
            series->append (trUtf8 ("Аварийные отказы"), 1);
            int A = RedmineInstance::instance ().metric8A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(0, 139, 139));
            slice->setLabel (trUtf8 ("<font size=4><b>Аварийные отказы - %1</b></font>").arg (QString::number (A)));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);


            series->append (trUtf8 ("Не аварийные отказы"), 2);
            int B = RedmineInstance::instance ().metric8B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("<font size=4><b>Не аварийные отказы - %1</b></font>").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(102, 205, 170));

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("<font size=6><b>Коэффициент аварийных отказов</b></font>"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            hl->addWidget (chartView);
        }

            //---------- Коэффициент отказов
        {
            QPieSeries *series = new QPieSeries ();
            series->append (trUtf8 ("Пройденные тест-кейсы исключений"), 1);
            int A = RedmineInstance::instance ().metric12A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(218, 112, 214));
            slice->setLabel (trUtf8 ("<font size=4><b>Пройденные тест-кейсы исключений - %1</b></font>").arg (QString::number (A)));

            series->append (trUtf8 ("Не пройденные тест-кейсы исключений"), 2);
            int B = RedmineInstance::instance ().metric12B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("<font size=4><b>Не пройденные тест-кейсы исключений - %1</b></font>").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(128,0,128));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("<font size=6><b>Коэффициент отказов</b></font>"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            hl->addWidget (chartView);
        }

        //--------- Предотвращение некорректных действий
        {
            QPieSeries *series = new QPieSeries ();
            series->append (trUtf8 ("Пройденные тест-кейсы некорректных действий "), 1);
            int A = RedmineInstance::instance ().metric13A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(135, 206, 235));
            slice->setLabel (trUtf8 ("<font size=4><b>Пройденные тест-кейсы некорректных действий - %1</b></font>").arg (QString::number (A)));

            series->append (trUtf8 ("Не пройденные проверку некорректных действий тест-кейсы"), 2);
            int B = RedmineInstance::instance ().metric13B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B-A);
            slice->setLabel (trUtf8 ("<font size=4><b>Не пройденные тест-кейсы некорректных действий - %1</b></font>").arg (QString::number (B-A)));
            slice->setExploded (true);
            slice->setColor(QColor(0,0,128));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("<font size=6><b>Предотвращение некорректных действий</b></font>"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}
