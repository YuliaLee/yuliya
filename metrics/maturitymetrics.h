#ifndef MATURITYMETRICS_H
#define MATURITYMETRICS_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

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

#endif // MATURITYMETRICS_H
