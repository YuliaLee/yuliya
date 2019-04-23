#include "faulttolerancemetrics.h"

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

FaultToleranceMetrics::FaultToleranceMetrics (const QString &prjid, QWidget *parent)
    : QWidget (parent)
{
    if (RedmineInstance::instance ().projects ().contains (prjid) &&
            RedmineInstance::instance ().loadIssues (prjid))
    {
        setWindowTitle (trUtf8 ("Внутренние метрики устойчивости к ошибкам"));

        QVBoxLayout *vl = new QVBoxLayout ();
        vl->setContentsMargins (4,4,4,4);
        setLayout (vl);

//        QHBoxLayout *hl = new QHBoxLayout ();
//        hl->setContentsMargins (4,4,4,4);
//        vl->addLayout (hl);

        //-- Способность к предотвращению некоррестных действий
        {
            int A = 0;
            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            for (int i = 0; i < project->_issues.size (); ++i)
            {
                //-- ищем задачи для предотращения некорректных действий
                if (project->_issues[i]->_tracker_id == "4" &&          //функционал
                        (project->_issues[i]->_category_id == "37" ||
                         project->_issues[i]->_category_id == "38" ||
                         project->_issues[i]->_category_id == "39" ||
                         project->_issues[i]->_category_id == "47" ||
                         project->_issues[i]->_category_id == "48" ||
                         project->_issues[i]->_category_id == "49" ||
                         project->_issues[i]->_category_id == "50" ||
                         project->_issues[i]->_category_id == "51" ||
                         project->_issues[i]->_category_id == "52" ||
                         project->_issues[i]->_category_id == "53") &&  //категория check test
                        (project->_issues[i]->_priority_id == "4" ||    //приоритет - срочный или немедленный
                         project->_issues[i]->_priority_id == "5"))
                {
                    A++;
                }
            }

            QBarSet *set0 = new QBarSet (trUtf8 ("Количество разработанных задач для предотвращения некорректных действий - %1").arg (QString::number (A)));
            *set0 << A;

            int B = project->_incorrect_actions;
            QBarSet *set1 = new QBarSet (trUtf8 ("Планируемое количество некорректных действий - %1").arg (QString::number (B)));
            *set1 << B;

            QBarSeries *series = new QBarSeries ();
            series->append (set0);
            series->append (set1);

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Способность к предотвращению некоррестных действий"));
            chart->setAnimationOptions (QChart::SeriesAnimations);

            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignBottom);

            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            vl->addWidget (chartView);
        }

        //-- Коэффициент отказов
        {
            QPieSeries *series = new QPieSeries();

            //-------------- Число предотвращённых исключений
            series->append (trUtf8 ("Число предотвращённых исключений"), 1);

            int A = 0;
            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            for (int i = 0; i < project->_issues.size (); ++i)
            {
                //-- ищем все решенные exception
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

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setLabel (trUtf8 ("Число предотвращённых исключений - %1").arg (QString::number (A)));

            //-------------- Число НЕ предотвращённых исключений
            series->append (trUtf8 ("Число НЕ предотвращённых исключений"), 2);

            int B = 0;
            project = RedmineInstance::instance ().projects ()[prjid];
            for (int i = 0; i < project->_issues.size (); ++i)
            {
                //-- ищем все ошибки
                if (project->_issues[i]->_tracker_id == "4" &&          //функционал
                        (project->_issues[i]->_category_id == "31" ||
                         project->_issues[i]->_category_id == "32" ||
                         project->_issues[i]->_category_id == "33" ||
                         project->_issues[i]->_category_id == "36"))  //категория exception
                {
                    B++;
                }
            }

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("Число НЕ предотвращённых исключений - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (4);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Коэффициент отказов"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }

    }
}

FaultToleranceMetrics::~FaultToleranceMetrics ()
{
}
