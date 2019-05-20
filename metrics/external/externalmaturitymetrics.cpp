#include "externalmaturitymetrics.h"

#include "redmineinstance.h"
#include "charts/drilldownchart.h"
#include "charts/drilldownslice.h"

#include <QDateTime>
#include <QDebug>
#include <QHBoxLayout>

#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QPieSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

using namespace External;

MaturityMetricsWidget1::MaturityMetricsWidget1 (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внешние метрики завершённости"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        {
            QLineSeries *series = new QLineSeries ();
            QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
            for (int i = 0; i < metrics.size (); ++i)
            {
                float X = (float)abs (RedmineInstance::instance ().projects ()[prjid]->_reference_number_of_error
                                      - metrics[i]._errors.toInt ()) / (float)metrics[i]._code_lines.toInt ();
                series->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
                series->setColor(QColor(75, 0, 130));
            }

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("<font size=5><b>Оцениваемая плотность скрытых ошибок</b></font>"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);
            chart->setToolTip(trUtf8("Все зависит от стадии испытаний. На поздних стадиях, чем меньше показатель, тем лучше."));

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }

        {
            QLineSeries *series = new QLineSeries ();
            QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
            for (int i = 0; i < metrics.size (); ++i)
            {
                float X = metrics[i]._errors.toFloat () / metrics[i]._test_cases.toFloat ();
                series->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
                series->setColor(QColor(32, 178, 170));
            }

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("<font size=5><b>Плотность ошибок по отношению к тест-кейсам</b></font>"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);
            chart->setToolTip(trUtf8("Все зависит от стадии испытаний. На поздних стадиях, чем меньше показатель, тем лучше."));

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }

        {
            QLineSeries *series = new QLineSeries ();
            QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
            for (int i = 0; i < metrics.size (); ++i)
            {
                float X = (float)metrics[i]._errors.toFloat () / metrics[i]._code_lines.toFloat ();
                series->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
                series->setColor(QColor(34, 139, 34));
            }

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("<font size=5><b>Плотность ошибок</b></font>"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight );
            chart->setToolTip(trUtf8("Все зависит от стадии испытаний. На поздних стадиях, чем меньше показатель, тем лучше."));

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
    else
        qCritical () << "[MaturityMetrics][MaturityMetrics] Could not find project"
                     << prjid;
}

MaturityMetricsWidget2::MaturityMetricsWidget2 (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внешние метрики завершённости"));

        QHBoxLayout *hl = new QHBoxLayout ();
        hl->setContentsMargins (4,4,4,4);
        setLayout (hl);

        //-- Обнаружение ошибок
        {
            int A = RedmineInstance::instance ().metric3A (prjid);

            QBarSet *set0 = new QBarSet (trUtf8 ("Число обнаруженных ошибок - %1").arg (QString::number (A)));
            *set0 << A;
            set0->setColor (QColor (255, 127, 80));

            {
                QFontMetrics fm (set0->labelFont ());
                if (_maxw0 < fm.width (trUtf8 (("Число обнаруженных ошибок - 666"))))
                    _maxw0 = fm.width (trUtf8 (("Число обнаруженных ошибок - 666")));
                _maxh0 += fm.height ();
            }

            int B = RedmineInstance::instance ().metric3B (prjid);
            QBarSet *set1 = new QBarSet (trUtf8 ("Планируемое число ошибок - %1").arg (QString::number (B)));
            *set1 << B;
            set1->setColor (QColor (255,69,0));

            {
                QFontMetrics fm (set1->labelFont ());
                if (_maxw0 < fm.width (trUtf8 (("Планируемое число ошибок - 666"))))
                    _maxw0 = fm.width (trUtf8 (("Планируемое число ошибок - 666")));
                _maxh0 += fm.height ();
            }

            QBarSeries *series = new QBarSeries ();
            series->append (set0);
            series->append (set1);

            _chart0 = new QChart ();
            _chart0->addSeries (series);
            _chart0->setTitle (trUtf8 ("<font size=5><b>Обнаружение ошибок</b></font>"));
            _chart0->setAnimationOptions (QChart::SeriesAnimations);
            _chart0->setToolTip(trUtf8("Сколько ошибок было обнаружено в исследуемом программном продукте?"));

            _chart0->legend ()->setVisible (true);
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

        //-- Устранение ошибок
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число решённых ошибок
            series->append (trUtf8 ("Число решённых ошибок"), 1);

            int A = RedmineInstance::instance ().metric4A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor (QColor (144, 238, 144));
            slice->setLabel (trUtf8 ("Решенные ошибки - %1").arg (QString::number (A)));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw1 < fm.width (trUtf8 (("Решенные ошибки - 666"))))
                    _maxw1 = fm.width (trUtf8 (("Решенные ошибки - 666")));
                _maxh1 += fm.height ();
            }

            //-------------- Число не решённых ошибок
            series->append (trUtf8 ("Число не решённых ошибок"), 2);

            int B = RedmineInstance::instance ().metric4B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не решённые ошибки - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor (QColor (0, 100, 0));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw1 < fm.width (trUtf8 (("Не решённые ошибки - 666"))))
                    _maxw1 = fm.width (trUtf8 (("Не решённые ошибки - 666")));
                _maxh1 += fm.height ();
            }

            //--------------

            _chart1 = new QChart ();
            _chart1->addSeries (series);
            _chart1->setTitle (trUtf8 ("<font size=5><b>Устранение ошибок</b></font>"));
            _chart1->setAnimationOptions (QChart::AllAnimations);
            _chart1->legend ()->setVisible (true);
            _chart1->legend ()->setAlignment (Qt::AlignRight);
            _chart1->setToolTip(trUtf8("Сколько найденных вовремя тестирования ошибок были исправлены? "));

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

        //        //-- Разрешение отказов
        //        {
        //            QPieSeries *series = new QPieSeries();

        //            //-------------- Число разрешенных отказов
        //            series->append (trUtf8 ("Число разрешенных отказов"), 1);

        //            int A = RedmineInstance::instance ().metric9A (prjid);

        //            QPieSlice *slice = series->slices ().at (0);
        //            slice->setValue (A);
        //            slice->setColor (QColor (233, 150, 122));
        //            slice->setLabel (trUtf8 ("<font size=3><b>Разрешенные отказы - %1</b></font>").arg (QString::number (A)));

        //            //-------------- Разрешенные отказы
        //            series->append (trUtf8 ("Не разрешенные отказы"), 2);

        //            int B = RedmineInstance::instance ().metric9B (prjid);

        //            slice = series->slices ().at (1);
        //            slice->setValue (B - A);
        //            slice->setLabel (trUtf8 ("<font size=3><b>Не разрешенные отказы - %1</b></font>").arg (QString::number (B - A)));
        //            slice->setExploded (true);
        //            slice->setColor (QColor(128, 0, 0));
        //            slice->setBorderColor (Qt::red);
        //            slice->setBorderWidth (3);

        //            //--------------

        //            QChart *chart = new QChart ();
        //            chart->addSeries (series);
        //            chart->setTitle (trUtf8 ("<font size=5><b>Разрешение отказов</b></font>"));
        //            chart->setAnimationOptions (QChart::AllAnimations);
        //            chart->legend ()->setVisible (true);
        //            chart->legend ()->setAlignment (Qt::AlignRight);

        //            QChartView *chartView = new QChartView (chart);
        //            chartView->setRenderHint (QPainter::Antialiasing);
        //            hl1->addWidget (chartView);
        //        }

        //        //-- Тестовое покрытие
        //        {
        //            QPieSeries *series = new QPieSeries();

        //            //-------------- Число запущенных тест-кейсов
        //            series->append (trUtf8 ("Запущенные тест-кейсы"), 1);

        //            int A = RedmineInstance::instance ().metric10A (prjid);

        //            QPieSlice *slice = series->slices ().at (0);
        //            slice->setValue (A);
        //            slice->setColor (QColor (255, 228, 181));
        //            slice->setLabel (trUtf8 ("<font size=3><b>Запущенные тест-кейсы - %1</b></font>").arg (QString::number (A)));

        //            //-------------- Не запущенные тест кейсы
        //            series->append (trUtf8 ("Не запущенные тест-кейсы"), 2);

        //            int B = RedmineInstance::instance ().metric5B (prjid);

        //            slice = series->slices ().at (1);
        //            slice->setValue (B - A);
        //            slice->setExploded (true);
        //            slice->setLabel (trUtf8 ("<font size=3><b>Не запущенные тест-кейсы - %1</b></font>").arg (QString::number (B - A)));
        //            slice->setColor(QColor(255, 215, 0));
        //            slice->setBorderColor (Qt::red);
        //            slice->setBorderWidth (3);

        //            //--------------

        //            QChart *chart = new QChart ();
        //            chart->addSeries (series);
        //            chart->setTitle (trUtf8 ("<font size=5><b>Тестовое покрытие</b></font>"));
        //            chart->setAnimationOptions (QChart::AllAnimations);
        //            chart->legend ()->setVisible (true);
        //            chart->legend ()->setAlignment (Qt::AlignRight);

        //            QChartView *chartView = new QChartView (chart);
        //            chartView->setRenderHint (QPainter::Antialiasing);
        //            hl1->addWidget (chartView);
        //        }

        //        //----------------Завершенность испытаний
        //        {
        //            QPieSeries *series = new QPieSeries();

        //            //-------------- Число прошедших тест-кейсов
        //            series->append (trUtf8 ("Прошедшие тест-кейсы"), 1);

        //            int A = RedmineInstance::instance ().metric11A (prjid);

        //            QPieSlice *slice = series->slices ().at (0);
        //            slice->setValue (A);
        //            slice->setColor (QColor (255, 182, 193));
        //            slice->setLabel (trUtf8 ("<font size=3><b>Прошедшие тест-кейсы - %1</b></font>").arg (QString::number (A)));

        //            //-------------- Не прошедшие тест кейсы
        //            series->append (trUtf8 ("Не прошедшие тест-кейсы"), 2);

        //            int B = RedmineInstance::instance ().metric5B (prjid);

        //            slice = series->slices ().at (1);
        //            slice->setValue (B - A);
        //            slice->setLabel (trUtf8 ("<font size=3><b>Не прошедшие тест-кейсы - %1</b></font>").arg (QString::number (B - A)));
        //            slice->setExploded (true);
        //            slice->setColor (QColor (199, 21, 133));
        //            slice->setBorderColor (Qt::red);
        //            slice->setBorderWidth (3);

        //            //--------------

        //            QChart *chart = new QChart ();
        //            chart->addSeries (series);
        //            chart->setTitle (trUtf8 ("<font size=5><b>Завершенность испытаний</b></font>"));
        //            chart->setAnimationOptions (QChart::AllAnimations);
        //            chart->legend ()->setVisible (true);
        //            chart->legend ()->setAlignment (Qt::AlignRight);

        //            QChartView *chartView = new QChartView (chart);
        //            chartView->setRenderHint (QPainter::Antialiasing);
        //            vl->addWidget (chartView);
        //        }
    }
}

void MaturityMetricsWidget2::resizeEvent (QResizeEvent *event)
{
    (void)event;

    if (_chart0)
        _chart0->legend ()->setGeometry (20, geometry ().bottom () - _maxh0 - 80, _maxw0 + 50, _maxh0 + 30);

    if (_chart1)
        _chart1->legend ()->setGeometry (20, geometry ().bottom () - _maxh1 - 80, _maxw1 + 50, _maxh1 + 30);
}

MaturityMetricsWidget3::MaturityMetricsWidget3 (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внешние метрики завершённости"));

        QHBoxLayout *hl = new QHBoxLayout ();
        hl->setContentsMargins (4,4,4,4);
        setLayout (hl);

        //-- Разрешение отказов
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число разрешенных отказов
            series->append (trUtf8 ("Число разрешенных отказов"), 1);

            int A = RedmineInstance::instance ().metric9A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor (QColor (233, 150, 122));
            slice->setLabel (trUtf8 ("Разрешенные отказы - %1").arg (QString::number (A)));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw0 < fm.width (trUtf8 (("Разрешенные отказы - 666"))))
                    _maxw0 = fm.width (trUtf8 (("Разрешенные отказы - 666")));
                _maxh0 += fm.height ();
            }

            //-------------- Разрешенные отказы
            series->append (trUtf8 ("Не разрешенные отказы"), 2);

            int B = RedmineInstance::instance ().metric9B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не разрешенные отказы - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor (QColor(128, 0, 0));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw0 < fm.width (trUtf8 (("Не разрешенные отказы - 666"))))
                    _maxw0 = fm.width (trUtf8 (("Не разрешенные отказы - 666")));
                _maxh0 += fm.height ();
            }

            //--------------

            _chart0 = new QChart ();
            _chart0->addSeries (series);
            _chart0->setTitle (trUtf8 ("<font size=5><b>Разрешение отказов</b></font>"));
            _chart0->setAnimationOptions (QChart::AllAnimations);
            _chart0->legend ()->setVisible (true);
            _chart0->legend ()->setAlignment (Qt::AlignRight);
            _chart0->setToolTip(trUtf8("Сколько состояний отказа удалось разрешить?"));

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

        //-- Тестовое покрытие
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число запущенных тест-кейсов
            series->append (trUtf8 ("Запущенные тест-кейсы"), 1);

            int A = RedmineInstance::instance ().metric10A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor (QColor (255, 228, 181));
            slice->setLabel (trUtf8 ("Запущенные тест-кейсы - %1").arg (QString::number (A)));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw1 < fm.width (trUtf8 (("Запущенные тест-кейсы - 666"))))
                    _maxw1 = fm.width (trUtf8 (("Запущенные тест-кейсы - 666")));
                _maxh1 += fm.height ();
            }

            //-------------- Не запущенные тест кейсы
            series->append (trUtf8 ("Не запущенные тест-кейсы"), 2);

            int B = RedmineInstance::instance ().metric5B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setExploded (true);
            slice->setLabel (trUtf8 ("Не запущенные тест-кейсы - %1").arg (QString::number (B - A)));
            slice->setColor(QColor(255, 215, 0));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw1 < fm.width (trUtf8 (("Не запущенные тест-кейсы - 666"))))
                    _maxw1 = fm.width (trUtf8 (("Не запущенные тест-кейсы - 666")));
                _maxh1 += fm.height ();
            }

            //--------------

            _chart1 = new QChart ();
            _chart1->addSeries (series);
            _chart1->setTitle (trUtf8 ("<font size=5><b>Тестовое покрытие</b></font>"));
            _chart1->setAnimationOptions (QChart::AllAnimations);
            _chart1->legend ()->setVisible (true);
            _chart1->legend ()->setAlignment (Qt::AlignRight);
            _chart1->setToolTip(trUtf8("Сколько требуемых тестов было выполнено во время проверки ПО?"));

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

        //----------------Завершенность испытаний
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число прошедших тест-кейсов
            series->append (trUtf8 ("Прошедшие тест-кейсы"), 1);

            int A = RedmineInstance::instance ().metric11A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor (QColor (255, 182, 193));
            slice->setLabel (trUtf8 ("Прошедшие тест-кейсы - %1").arg (QString::number (A)));

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw2 < fm.width (trUtf8 (("Прошедшие тест-кейсы - 666"))))
                    _maxw2 = fm.width (trUtf8 (("Прошедшие тест-кейсы - 666")));
                _maxh2 += fm.height ();
            }

            //-------------- Не прошедшие тест кейсы
            series->append (trUtf8 ("Не прошедшие тест-кейсы"), 2);

            int B = RedmineInstance::instance ().metric5B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не прошедшие тест-кейсы - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor (QColor (199, 21, 133));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            {
                QFontMetrics fm (slice->labelFont ());
                if (_maxw2 < fm.width (trUtf8 (("Не прошедшие тест-кейсы - 666"))))
                    _maxw2 = fm.width (trUtf8 (("Не прошедшие тест-кейсы - 666")));
                _maxh2 += fm.height ();
            }

            //--------------

            _chart2 = new QChart ();
            _chart2->addSeries (series);
            _chart2->setTitle (trUtf8 ("<font size=5><b>Завершенность испытаний</b></font>"));
            _chart2->setAnimationOptions (QChart::AllAnimations);
            _chart2->legend ()->setVisible (true);
            _chart2->legend ()->setAlignment (Qt::AlignRight);
            _chart2->setToolTip(trUtf8("Хорошо ли проведено тестирование программного обеспечения?"));


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

void MaturityMetricsWidget3::resizeEvent (QResizeEvent *event)
{
    (void)event;

    if (_chart0)
        _chart0->legend ()->setGeometry (20, geometry ().bottom () - _maxh0 - 80, _maxw0 + 50, _maxh0 + 30);

    if (_chart1)
        _chart1->legend ()->setGeometry (20, geometry ().bottom () - _maxh1 - 80, _maxw1 + 50, _maxh1 + 30);

    if (_chart2)
        _chart2->legend ()->setGeometry (20, geometry ().bottom () - _maxh2 - 80, _maxw2 + 50, _maxh2 + 30);
}
