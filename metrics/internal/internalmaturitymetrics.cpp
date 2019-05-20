#include "internalmaturitymetrics.h"

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

using namespace Internal;

MaturityMetricsWidget::MaturityMetricsWidget (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внутренние метрики завершённости"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        QHBoxLayout *hl = new QHBoxLayout ();
        hl->setContentsMargins (4,4,4,4);
        vl->addLayout (hl);

        //-- Адекватность теста
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Кол-во разработанных тест-кейсов
            series->append (trUtf8 ("Разработанные тест-кейсы:"), 1);

            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            int A = RedmineInstance::instance ().metric5B (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setLabel (trUtf8 ("Разработанные тест-кейсы - %1").arg (QString::number (A)));
            slice->setValue (A);
            slice->setColor (QColor (135, 206, 250));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw < fm.width (trUtf8 (("Разработанные тест-кейсы - 666"))))
                    _maxw = fm.width (trUtf8 (("Разработанные тест-кейсы - 666")));
                _maxh += fm.height ();
            }

            //-------------- Не разработанное кол-во тест-кейсов
            series->append (trUtf8 ("Не разработанные тест-кейсы:"), 2);

            int B = project->_need_test_case;

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не разработанные тест-кейсы - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(30, 144, 255));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw < fm.width (trUtf8 (("Не разработанные тест-кейсы - 666"))))
                    _maxw = fm.width (trUtf8 (("Не разработанные тест-кейсы - 666")));
                _maxh += fm.height ();
            }

            //--------------

            _chart = new QChart ();
            _chart->addSeries (series);
            _chart->setTitle (trUtf8 ("<font size=6><b>Адекватность теста</b></font>"));
            _chart->setAnimationOptions (QChart::AllAnimations);
            _chart->legend ()->setVisible (true);
            _chart->legend ()->setAlignment (Qt::AlignRight);
            _chart->setToolTip(trUtf8("Сколько необходимых тест-кейсов было разработано? "));

            _chart->legend ()->detachFromChart ();
            _chart->legend ()->setBackgroundVisible (true);
            _chart->legend ()->setAlignment (Qt::AlignLeft);

            QChartView *chartView = new QChartView (_chart);
            chartView->setRenderHint (QPainter::Antialiasing);

            vl->addWidget (chartView);
        }
    }
    else
        qCritical () << "[OneChartWidget][OneChartWidget] Could not find project"
                     << prjid;
}

MaturityMetricsWidget::~MaturityMetricsWidget ()
{}

void MaturityMetricsWidget::resizeEvent (QResizeEvent *event)
{
    (void)event;

    if (!_chart)
        return;

    _chart->legend ()->setGeometry (20, geometry ().bottom () - _maxh - 80, _maxw + 50, _maxh + 30);
}
