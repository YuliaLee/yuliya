#include "resultingexternalmetrics.h"

#include "redmineinstance.h"
#include "charts/drilldownchart.h"
#include "charts/drilldownslice.h"

#include <QDateTime>
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
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

ResultingExternalMetricsWidget::ResultingExternalMetricsWidget(const QString &prjid, QWidget *parent)
    : QWidget(parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        setWindowTitle (trUtf8 ("Результирующие графики внешних метрик"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        QBarSeries *series = new QBarSeries ();

        QValueAxis *axisY = new QValueAxis ();
        axisY->setRange (0, 1);
        axisY->applyNiceNumbers ();
        series->attachAxis (axisY);

        //------------------- Обнаружение ошибок
        {
            int A = RedmineInstance::instance ().metric3A (prjid);
            int B = RedmineInstance::instance ().metric3B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Обнаружение ошибок - %1</b></font>").arg (QString::number (X, 'f', 2)));
            *set << X;
            series->append (set);
            set->setColor(QColor(255,69,0));
        }

        //------------------- Устранение ошибок
        {
            int A = RedmineInstance::instance ().metric4A (prjid);
            int B = RedmineInstance::instance ().metric4B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Устранение ошибок - %1</b></font>").arg (QString::number (X, 'f', 2)));
            *set << X;
            series->append (set);
            set->setColor(QColor(0, 100, 0));
        }

        //------------------- Разрешение отказов
        {
            int A = RedmineInstance::instance ().metric9A (prjid);
            int B = RedmineInstance::instance ().metric9B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Разрешение отказов - %1</b></font>").arg (QString::number (X, 'f', 2)));
            *set << X;
            series->append (set);
            set->setColor(QColor(128, 0, 0));
        }

        //------------------- Тестовое покрытие
        {
            int A = RedmineInstance::instance ().metric10A (prjid);
            int B = RedmineInstance::instance ().metric5B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Тестовое покрытие - %1</b></font>").arg (QString::number (X, 'f', 2)));
            *set << X;
            series->append (set);
            set->setColor(QColor(255, 215, 0));
        }

        //------------------- Завершенность испытаний
        {
            int A = RedmineInstance::instance ().metric11A (prjid);
            int B = RedmineInstance::instance ().metric5B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Завершенность испытаний - %1</b></font>").arg (QString::number (X, 'f', 2)));
            *set << X;
            series->append (set);
            set->setColor(QColor(199, 21, 133));
        }

        //------------------- Коэффициент аварийных отказов
        {
            int A = RedmineInstance::instance ().metric8A (prjid);
            int B = RedmineInstance::instance ().metric8B (prjid);
            float X = 0;
            if (B != 0)
                X = (1-(float)A/(float)B);
            else
                X = 1;

            QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Коэффициент аварийных отказов - %1</b></font>").arg (QString::number (X, 'f', 2)));
            *set << X;
            series->append (set);
            set->setColor(QColor(0, 139, 139));
        }

        //------------------- Коэффициент отказов
        {
            int A = RedmineInstance::instance ().metric12A (prjid);
            int B = RedmineInstance::instance ().metric12B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Коэффициент отказов - %1</b></font>").arg (QString::number (X, 'f', 2)));
            *set << X;
            series->append (set);
            set->setColor(QColor(128,0,128));
        }

        //------------------- Предотвращение некорректных действий
        {
            int A = RedmineInstance::instance ().metric13A (prjid);
            int B = RedmineInstance::instance ().metric13B (prjid);
            float X = 0;
            if (B != 0)
                X = (float)A/(float)B;
            else
                X = 1;

            QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Предотвращение некорр. действий - %1</b></font>").arg (QString::number (X, 'f', 2)));
            *set << X;
            series->append (set);
            set->setColor(QColor(0,0,128));
        }
//        {
//            QLineSeries *series1 = new QLineSeries ();
//            {
//                series1->setName (trUtf8 ("<font size=5><b>Оцениваемая плотность скрытых ошибок - %1</b></font>"));
//                QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
//                for (int i = 0; i < metrics.size (); ++i)
//                {
//                    float X = (float)abs (RedmineInstance::instance ().projects ()[prjid]->_reference_number_of_error
//                                          - metrics[i]._errors.toInt ()) / (float)metrics[i]._code_lines.toInt ();
//                    series1->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
//                }
//            }

//            QLineSeries *series2 = new QLineSeries ();
//            {
//                series2->setName (trUtf8 ("<font size=5><b>Плотность отказов по отношению к тест-кейсам - %1</b></font>"));
//                QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
//                for (int i = 0; i < metrics.size (); ++i)
//                {
//                    float X = metrics[i]._errors.toFloat () / metrics[i]._test_cases.toFloat ();
//                    series2->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
//                }
//            }

//            QLineSeries *series3 = new QLineSeries ();
//            {
//                series3->setName (trUtf8 ("<font size=5><b>Плотность ошибок - %1</b></font>"));
//                QList<RedmineProjectCodeMetrics> metrics = RedmineInstance::instance ().codeMetrics (prjid);
//                for (int i = 0; i < metrics.size (); ++i)
//                {
//                    float X = (float)metrics[i]._errors.toFloat () / metrics[i]._code_lines.toFloat ();
//                    series3->append (QDateTime::fromString (metrics[i]._date).toTime_t (), X);
//                    //qDebug () << (float)metrics[i]._errors.toFloat () << metrics[i]._code_lines.toFloat () << X;
//                }
//            }


            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->addAxis (axisY, Qt::AlignLeft);
            chart->setTitle (trUtf8 ("<font size=18><b>Результирующий график внешних метрик</b></font>"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);
            chart->setToolTip(trUtf8("Чем ближе показатель к 1, тем лучше"));

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
    }
}
