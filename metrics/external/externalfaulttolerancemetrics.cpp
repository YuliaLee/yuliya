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
            slice->setColor (QColor (0, 139, 139));
            slice->setLabel (trUtf8 ("Аварийные отказы - %1").arg (QString::number (A)));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw0 < fm.width (trUtf8 (("Аварийные отказы - 666"))))
                    _maxw0 = fm.width (trUtf8 (("Аварийные отказы - 666")));
                _maxh0 += fm.height ();
            }

            series->append (trUtf8 ("Не аварийные отказы"), 2);
            int B = RedmineInstance::instance ().metric8B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не аварийные отказы - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor (QColor (102, 205, 170));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw0 < fm.width (trUtf8 (("Не аварийные отказы - 666"))))
                    _maxw0 = fm.width (trUtf8 (("Не аварийные отказы - 666")));
                _maxh0 += fm.height ();
            }

            //--------------

            _chart0 = new QChart ();
            _chart0->addSeries (series);
            _chart0->setTitle (trUtf8 ("<font size=5><b>Коэффициент аварийных отказов</b></font>"));
            _chart0->setAnimationOptions (QChart::AllAnimations);

            _chart0->legend ()->setVisible (true);
            _chart0->legend ()->setAlignment (Qt::AlignRight);
            _chart0->legend ()->detachFromChart ();
            _chart0->legend ()->setBackgroundVisible (true);
            _chart0->legend ()->setAlignment (Qt::AlignLeft);

            QFont f = _chart0->legend ()->font ();
            f.setPixelSize (14);
            _chart0->legend ()->setFont (f);

            QChartView *chartView = new QChartView (_chart0);
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
            slice->setColor (QColor (218, 112, 214));
            slice->setLabel (trUtf8 ("Пройденные тест-кейсы исключений - %1").arg (QString::number (A)));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw1 < fm.width (trUtf8 (("Пройденные тест-кейсы исключений - 666"))))
                    _maxw1 = fm.width (trUtf8 (("Пройденные тест-кейсы исключений - 666")));
                _maxh1 += fm.height ();
            }

            series->append (trUtf8 ("Не пройденные тест-кейсы исключений"), 2);
            int B = RedmineInstance::instance ().metric12B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не пройденные тест-кейсы исключений - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor (QColor (128,0,128));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw1 < fm.width (trUtf8 (("Не пройденные тест-кейсы исключений - 666"))))
                    _maxw1 = fm.width (trUtf8 (("Не пройденные тест-кейсы исключений - 666")));
                _maxh1 += fm.height ();
            }

            //--------------

            _chart1 = new QChart ();
            _chart1->addSeries (series);
            _chart1->setTitle (trUtf8 ("<font size=5><b>Коэффициент отказов</b></font>"));
            _chart1->setAnimationOptions (QChart::AllAnimations);
            _chart1->legend ()->setVisible (true);
            _chart1->legend ()->setAlignment (Qt::AlignRight);

            _chart1->legend ()->detachFromChart ();
            _chart1->legend ()->setBackgroundVisible (true);
            _chart1->legend ()->setAlignment (Qt::AlignLeft);

            QFont f = _chart1->legend ()->font ();
            f.setPixelSize (14);
            _chart1->legend ()->setFont (f);

            QChartView *chartView = new QChartView (_chart1);
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
            slice->setColor (QColor (135, 206, 235));
            slice->setLabel (trUtf8 ("Пройденные тест-кейсы некорректных действий - %1").arg (QString::number (A)));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw2 < fm.width (trUtf8 (("Пройденные тест-кейсы некорректных действий - 666"))))
                    _maxw2 = fm.width (trUtf8 (("Пройденные тест-кейсы некорректных действий - 666")));
                _maxh2 += fm.height ();
            }

            series->append (trUtf8 ("Не пройденные проверку некорректных действий тест-кейсы"), 2);
            int B = RedmineInstance::instance ().metric13B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B-A);
            slice->setLabel (trUtf8 ("Не пройденные тест-кейсы некорректных действий - %1").arg (QString::number (B-A)));
            slice->setExploded (true);
            slice->setColor (QColor (0,0,128));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw2 < fm.width (trUtf8 (("Не пройденные тест-кейсы некорректных действий - 666"))))
                    _maxw2 = fm.width (trUtf8 (("Не пройденные тест-кейсы некорректных действий - 666")));
                _maxh2 += fm.height ();
            }

            //--------------

            _chart2 = new QChart ();
            _chart2->addSeries (series);
            _chart2->setTitle (trUtf8 ("<font size=5><b>Предотвращение некорректных действий</b></font>"));
            _chart2->setAnimationOptions (QChart::AllAnimations);
            _chart2->legend ()->setVisible (true);
            _chart2->legend ()->setAlignment (Qt::AlignRight);

            _chart2->legend ()->detachFromChart ();
            _chart2->legend ()->setBackgroundVisible (true);
            _chart2->legend ()->setAlignment (Qt::AlignLeft);

            QFont f = _chart2->legend ()->font ();
            f.setPixelSize (14);
            _chart2->legend ()->setFont (f);

            QChartView *chartView = new QChartView (_chart2);
            chartView->setRenderHint (QPainter::Antialiasing);
            hl->addWidget (chartView);
        }
    }
}

void FaultToleranceMetrics::resizeEvent (QResizeEvent *event)
{
    (void)event;

    if (_chart0)
        _chart0->legend ()->setGeometry (20, geometry ().bottom () - _maxh0 - 80, _maxw0 + 50, _maxh0 + 30);

    if (_chart1)
        _chart1->legend ()->setGeometry (20, geometry ().bottom () - _maxh1 - 80, _maxw1 + 50, _maxh1 + 30);

    if (_chart2)
        _chart2->legend ()->setGeometry (20, geometry ().bottom () - _maxh2 - 80, _maxw2 + 50, _maxh2 + 30);
}
