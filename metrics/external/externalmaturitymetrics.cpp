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
            chart->setTitle (trUtf8 ("Оцениваемая плотность скрытых ошибок"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

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
            chart->setTitle (trUtf8 ("Плотность отказов по отношению к тест-кейсам"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

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
            chart->setTitle (trUtf8 ("Плотность ошибок"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->createDefaultAxes ();
            chart->axisY ()->setMin (0);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight );

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

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        QHBoxLayout *hl0 = new QHBoxLayout ();
        hl0->setContentsMargins (4,4,4,4);
        vl->addLayout (hl0);

        QHBoxLayout *hl1 = new QHBoxLayout ();
        hl1->setContentsMargins (4,4,4,4);
        vl->addLayout (hl1);


        //-- Обнаружение ошибок
        {
            int A = RedmineInstance::instance ().metric3A (prjid);

            QBarSet *set0 = new QBarSet (trUtf8 ("Число обнаруженных ошибок - %1").arg (QString::number (A)));
            *set0 << A;
            set0->setColor(QColor(255, 127, 80));

            int B = RedmineInstance::instance ().metric3B (prjid);
            //            int B = project->_reference_number_of_error;
            QBarSet *set1 = new QBarSet (trUtf8 ("Планируемое число ошибок - %1").arg (QString::number (B)));
            *set1 << B;
            set1->setColor(QColor(255,69,0));

            QBarSeries *series = new QBarSeries ();
            series->append (set0);
            series->append (set1);

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Обнаружение ошибок"));
            chart->setAnimationOptions (QChart::SeriesAnimations);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            hl0->addWidget (chartView);
        }

        //-- Устранение ошибок
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число решённых ошибок
            series->append (trUtf8 ("Число решённых ошибок"), 1);

            int A = RedmineInstance::instance ().metric4A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(144, 238, 144));
            slice->setLabel (trUtf8 ("Решенные ошибки - %1").arg (QString::number (A)));

            //-------------- Число не решённых ошибок
            series->append (trUtf8 ("Число не решённых ошибок"), 2);

            int B = RedmineInstance::instance ().metric4B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не решённые ошибки - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(0, 100, 0));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Устранение ошибок"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            hl0->addWidget (chartView);
        }

        //-- Разрешение отказов
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число разрешенных отказов
            series->append (trUtf8 ("Число разрешенных отказов"), 1);

            int A = RedmineInstance::instance ().metric9A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(233, 150, 122));
            slice->setLabel (trUtf8 ("Разрешенные отказы - %1").arg (QString::number (A)));

            //-------------- Разрешенные отказы
            series->append (trUtf8 ("Не разрешенные отказы"), 2);

            int B = RedmineInstance::instance ().metric9B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не разрешенные отказы - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(128, 0, 0));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Разрешение отказов"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            hl1->addWidget (chartView);
        }

        {
        //-- Тестовое покрытие
            QPieSeries *series = new QPieSeries();

            //-------------- Число запущенных тест-кейсов
            series->append (trUtf8 ("Запущенные тест-кейсы"), 1);

            int A = RedmineInstance::instance ().metric10A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(255, 228, 181));
            slice->setLabel (trUtf8 ("Запущенные тест-кейсы- %1").arg (QString::number (A)));

            //-------------- Не запущенные тест кейсы
            series->append (trUtf8 ("Не запущенные тест-кейсы"), 2);

            int B = RedmineInstance::instance ().metric5B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не запущенные тест-кейсы - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(255, 215, 0));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Тестовое покрытие"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            hl1->addWidget (chartView);
        }

        //----------------Завершенность испытаний
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число прошедших тест-кейсов
            series->append (trUtf8 ("Прошедшие тест-кейсы"), 1);

            int A = RedmineInstance::instance ().metric11A (prjid);

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setColor(QColor(255, 182, 193));
            slice->setLabel (trUtf8 ("Прошедшие тест-кейсы- %1").arg (QString::number (A)));

            //-------------- Не прошедшие тест кейсы
            series->append (trUtf8 ("Не прошедшие тест-кейсы"), 2);

            int B = RedmineInstance::instance ().metric5B (prjid);

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Не прошедшие тест-кейсы - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setColor(QColor(199, 21, 133));
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (3);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Завершенность испытаний"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}
