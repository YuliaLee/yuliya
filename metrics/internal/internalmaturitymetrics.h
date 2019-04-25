#ifndef INTERNAL_MATURITYMETRICS_H
#define INTERNAL_MATURITYMETRICS_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

namespace Internal
{

//!
//! \brief Внутренние метрики завершённости (maturitymetrics)
//!
class MaturityMetricsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MaturityMetricsWidget (const QString &prjid, QWidget *parent = nullptr);
    virtual ~MaturityMetricsWidget ();
};

}

#endif //INTERNAL_MATURITYMETRICS_H
