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

QT_CHARTS_USE_NAMESPACE

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

            int A = 0;
            QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[prjid];
            for (int i = 0; i < project->_issues.size (); ++i)
            {
                //-- ищем все ошибки
                if (project->_issues[i]->_tracker_id == "4" &&          //функционал
                        project->_issues[i]->_status_id == "3")         //статус - решена
                {
                    QList<QSharedPointer<RedmineAttachments> > attachments
                            = RedmineInstance::instance ().attachments ().values ();

                    for (int j = 0; j < attachments.size (); ++j)
                    {
                        if (attachments[j]->_container_type == "Issue" &&
                                attachments[j]->_container_id == project->_issues[i]->_id &&
                                (attachments[j]->_filename.toLower ().contains (trUtf8 ("надёжност")) ||
                                 attachments[j]->_filename.toLower ().contains (trUtf8 ("надежност"))))
                            A++;
                    }
                }
            }

            QPieSlice *slice = series->slices ().at (0);
            slice->setValue (A);
            slice->setLabel (trUtf8 ("A - %1").arg (QString::number (A)));

            //-------------- Число не решённых ошибок
            series->append (trUtf8 ("B"), 2);

            int B = 0;
            project = RedmineInstance::instance ().projects ()[prjid];
            for (int i = 0; i < project->_issues.size (); ++i)
            {
                //-- ищем все ошибки
                if (project->_issues[i]->_tracker_id == "4")          //функционал
                {
                    QList<QSharedPointer<RedmineAttachments> > attachments
                            = RedmineInstance::instance ().attachments ().values ();

                    for (int j = 0; j < attachments.size (); ++j)
                    {
                        if (attachments[j]->_container_type == "Issue" &&
                                attachments[j]->_container_id == project->_issues[i]->_id &&
                                (attachments[j]->_filename.toLower ().contains (trUtf8 ("надёжност")) ||
                                 attachments[j]->_filename.toLower ().contains (trUtf8 ("надежност"))))
                            B++;
                    }
                }
            }

            slice = series->slices ().at (1);
            slice->setValue (B - A);
            slice->setLabel (trUtf8 ("B - %1").arg (QString::number (B - A)));
            slice->setExploded (true);
            slice->setBorderColor (Qt::red);
            slice->setBorderWidth (4);

            //--------------

            QChart *chart = new QChart ();
            chart->addSeries (series);
            chart->setTitle (trUtf8 ("Внутренняя метрика соответсвия надёжности"));
            chart->setAnimationOptions (QChart::AllAnimations);
            chart->legend ()->setVisible (true);
            chart->legend ()->setAlignment (Qt::AlignRight);

            QChartView *chartView = new QChartView (chart);
            chartView->setRenderHint (QPainter::Antialiasing);
            vl->addWidget (chartView);
        }
    }
}
