#ifndef INTERNAL_MATURITYMETRICS_H
#define INTERNAL_MATURITYMETRICS_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

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

protected:
    void resizeEvent (QResizeEvent *event);

    int _maxw = 0;
    int _maxh = 0;
    QChart *_chart = nullptr;
};

}

#endif //INTERNAL_MATURITYMETRICS_H
