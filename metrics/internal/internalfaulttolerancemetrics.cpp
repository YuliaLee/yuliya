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

using namespace Internal;

FaultToleranceMetrics::FaultToleranceMetrics (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        setWindowTitle (trUtf8 ("Внутренние метрики устойчивости к ошибкам"));

        QHBoxLayout *lay = new QHBoxLayout ();
        lay->setContentsMargins (4,4,4,4);
        setLayout (lay);

        //-- Способность к предотвращению некоррестных действий
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Количество предотвращенных некорректных действий
            series->append (trUtf8 ("Предотвращенные некорректные действия"), 1);

            int A = RedmineInstance::instance ().metric1A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor (QColor (255, 160, 122));
            slice->setLabel (trUtf8 (("Предотвращенные некорректные действия - %1")).arg (QString::number (A)));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw0 < fm.width (trUtf8 (("Предотвращенные некорректные действия - 666"))))
                    _maxw0 = fm.width (trUtf8 (("Предотвращенные некорректные действия - 666")));
                _maxh0 += fm.height ();
            }

            //-------------- Количество Не разработанных задач для предотвращения некорректных действий
            series->append (trUtf8 ("Не предотвращенные некорректные действия"), 2);

            int B = RedmineInstance::instance ().metric1B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не предотвращенные некорректные действия - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor (QColor (255, 140, 0));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw0 < fm.width (trUtf8 (("Не предотвращенные некорректные действия - 666"))))
                    _maxw0 = fm.width (trUtf8 (("Не предотвращенные некорректные действия - 666")));
                _maxh0 += fm.height ();
            }

            //--------------

            _chart0 = new QChart ();
            _chart0->addSeries (series);
            _chart0->setTitle (trUtf8 ("<font size=6><b>Предотвращение некорректных действий</b></font>"));
            _chart0->setAnimationOptions (QChart::AllAnimations);
            _chart0->legend ()->setVisible (true);
            _chart0->legend ()->setAlignment (Qt::AlignBottom);

            _chart0->legend ()->detachFromChart ();
            _chart0->legend ()->setBackgroundVisible (true);
            _chart0->legend ()->setAlignment (Qt::AlignLeft);

            QFont f = _chart0->legend ()->font ();
            f.setPixelSize (14);
            _chart0->legend ()->setFont (f);

            QChartView *chartView = new QChartView (_chart0);
            chartView->setRenderHint (QPainter::Antialiasing);
            lay->addWidget (chartView);
        }

        //-- Коэффициент отказов
        {
            QPieSeries *series = new QPieSeries ();

            //-------------- Число предотвращённых исключений
            series->append (trUtf8 ("Предотвращённые исключения"), 1);

            int A = RedmineInstance::instance ().metric2A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor (QColor (124, 252, 0));
            slice->setLabel (trUtf8 ("Предотвращённые исключения - %1").arg (QString::number (A)));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw1 < fm.width (trUtf8 (("Предотвращённые исключения - 666"))))
                    _maxw1 = fm.width (trUtf8 (("Предотвращённые исключения - 666")));
                _maxh1 += fm.height ();
            }

            //-------------- Число НЕ предотвращённых исключений
            series->append (trUtf8 ("Не предотвращённые исключения"), 2);

            int B = RedmineInstance::instance ().metric2B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не предотвращённые исключения - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor (QColor (154, 205, 50));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw1 < fm.width (trUtf8 (("Не предотвращённые исключения - 666"))))
                    _maxw1 = fm.width (trUtf8 (("Не предотвращённые исключения - 666")));
                _maxh1 += fm.height ();
            }

            //--------------

            _chart1 = new QChart ();
            _chart1->addSeries (series);
            _chart1->setTitle (trUtf8 ("<font size=6><b>Коэффициент отказов</b></font>"));
            _chart1->setAnimationOptions (QChart::AllAnimations);
            _chart1->legend ()->setVisible (true);
            _chart1->legend ()->setAlignment (Qt::AlignBottom);

            _chart1->legend ()->detachFromChart ();
            _chart1->legend ()->setBackgroundVisible (true);
            _chart1->legend ()->setAlignment (Qt::AlignLeft);

            QFont f = _chart1->legend ()->font ();
            f.setPixelSize (14);
            _chart1->legend ()->setFont (f);

            QChartView *chartView = new QChartView (_chart1);
            chartView->setRenderHint (QPainter::Antialiasing);
            lay->addWidget (chartView);
        }
    }
}

FaultToleranceMetrics::~FaultToleranceMetrics ()
{}

void FaultToleranceMetrics::resizeEvent (QResizeEvent *event)
{
    (void)event;

    if (_chart0)
        _chart0->legend ()->setGeometry (20, geometry ().bottom () - _maxh0 - 80, _maxw0 + 50, _maxh0 + 30);
    if (_chart1)
        _chart1->legend ()->setGeometry (20, geometry ().bottom () - _maxh1 - 80, _maxw1 + 50, _maxh1 + 30);
}
