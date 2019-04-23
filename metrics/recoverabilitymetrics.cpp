#include "recoverabilitymetrics.h"

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

RecoverabilityMetrics::RecoverabilityMetrics (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внутренние метрики восстанавливаемости"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

        //-- Восстанавливаемость
        {
            int A = 0;
            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            for (int i = 0; i < project->_issues.size (); ++i)
            {
                //-- ищем все ошибки
                if (project->_issues[i]->_tracker_id == "4" &&          //функционал
                        (project->_issues[i]->_category_id == "31" ||
                         project->_issues[i]->_category_id == "32" ||
                         project->_issues[i]->_category_id == "33" ||
                         project->_issues[i]->_category_id == "36") &&  //категория exception
                        project->_issues[i]->_status_id == "3")         //статус - решена
                {
                    A++;
                }
            }

            QBarSet *set0 = new QBarSet (trUtf8 ("Число предотвращённых исключений - %1").arg (QString::number (A)));
            *set0 << A;

            int B = project->_exceptions;
            QBarSet *set1 = new QBarSet (trUtf8 ("Планируемое количество исключений - %1").arg (QString::number (B)));
            *set1 << B;

            QBarSeries *series = new QBarSeries ();
            series->append (set0);
            series->append (set1);

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Восстанавливаемость"));
            chart->setAnimationOptions (QChart::SeriesAnimations);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}
