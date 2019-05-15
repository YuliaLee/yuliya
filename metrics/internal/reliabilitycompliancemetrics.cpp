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
            slice->setColor (QColor (240, 128, 128));
            slice->setLabel (trUtf8 ("Соответствующие надежности задачи - %1").arg (QString::number (A)));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw < fm.width (trUtf8 (("Соответствующие надежности задачи - 666"))))
                    _maxw = fm.width (trUtf8 (("Соответствующие надежности задачи - 666")));
                _maxh += fm.height ();
            }

            //-------------- Число не решённых ошибок
            series->append (trUtf8 ("B"), 2);

            int B = RedmineInstance::instance ().metric7B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не соответствующие надежности задачи - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(178, 34, 34));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw < fm.width (trUtf8 (("Не соответствующие надежности задачи - 666"))))
                    _maxw = fm.width (trUtf8 (("Не соответствующие надежности задачи - 666")));
                _maxh += fm.height ();
            }

            //--------------

            _chart = new QChart ();
            _chart->addSeries (series);
            _chart->setTitle (trUtf8 ("<font size=6><b>Соответсвие надёжности</b></font>"));
            _chart->setAnimationOptions (QChart::AllAnimations);
            _chart->legend ()->setVisible (true);
            _chart->legend ()->setAlignment (Qt::AlignRight);

            _chart->legend ()->detachFromChart ();
            _chart->legend ()->setBackgroundVisible (true);
            _chart->legend ()->setAlignment (Qt::AlignLeft);

            QFont f = _chart->legend ()->font ();
            f.setPixelSize (14);
            _chart->legend ()->setFont (f);

            QChartView *chartView = new QChartView (_chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}

void ReliabilityComplianceMetrics::resizeEvent (QResizeEvent *event)
{
    (void)event;

    if (!_chart)
        return;

    _chart->legend ()->setGeometry (20, geometry ().bottom () - _maxh - 80, _maxw + 50, _maxh + 30);
}
