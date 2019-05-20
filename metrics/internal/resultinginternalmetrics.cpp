#include "resultinginternalmetrics.h"

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
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegendMarker>

QT_CHARTS_USE_NAMESPACE

ResultingInternalMetricsWidget::ResultingInternalMetricsWidget (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid) &&
            RedmineInstance::instance ().loadAttachments ())
    {
        setWindowTitle (trUtf8 ("Результирующий график внутренних метрик"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        {
            QBarSeries *series = new QBarSeries ();

            QValueAxis *axisY = new QValueAxis ();
            axisY->setRange (0, 1);
            axisY->applyNiceNumbers ();
            series->attachAxis (axisY);

            //-- Test adequance - Адекватность теста
            {
                QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
                int A = RedmineInstance::instance ().metric5B (prjid);
                int B = project->_need_test_case;
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 0;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Адекватность теста  - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            //-- Failure avoidance - Коэффициент отказов
            {
                int A = RedmineInstance::instance ().metric2A (prjid);
                int B = RedmineInstance::instance ().metric2B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Коэффициент отказов - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            //-- Incorrect operation avoidance - Способность к предотвращению некорректных действий
            {
                int A = RedmineInstance::instance ().metric1A (prjid);
                int B = RedmineInstance::instance ().metric1B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Предотвращение некорр. действий - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            //-- Restorability - Восстанавливаемость
            {
                int A = RedmineInstance::instance ().metric6A (prjid);
                int B = RedmineInstance::instance ().metric6B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Восстанавливаемость - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            //-- Reliability Compliance - Соответствие надежности
            {
                int A = RedmineInstance::instance ().metric7A (prjid);
                int B = RedmineInstance::instance ().metric7B (prjid);
                float X = 0;
                if (B != 0)
                    X = (float)A/(float)B;
                else
                    X = 1;

                QBarSet *set = new QBarSet (trUtf8 ("<font size=5><b>Соответствие надежности - %1</b></font>").arg (QString::number (X, 'f', 2)));
                *set << X;
                series->append (set);
            }

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->addAxis (axisY, Qt::AlignLeft);
            chart->setTitle (trUtf8 ("<font size=18><b>Результирующий график внутренних метрик</b></font>"));
            chart->setAnimationOptions (QChart::SeriesAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);
            chart->setToolTip(trUtf8("Чем ближе показатель к 1, тем лучше, так как 1 свидетельствует, что все поставленные в требованиях задачи были выполнены."));

            QList <QLegendMarker*> markers = chart->legend ()->markers ();
            for (int i = 0; i < markers.size (); ++i)
                connect (markers[i], SIGNAL(clicked()), SLOT(slotMarkerClicked()));

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}

void ResultingInternalMetricsWidget::slotMarkerClicked ()
{
    QLegendMarker *marker = qobject_cast<QLegendMarker *> (sender ());
    if (!marker)
        return;

//    qDebug () << marker->label ();
}
