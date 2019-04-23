#ifndef RESULTINGCHARTOFMETRICS_H
#define RESULTINGCHARTOFMETRICS_H

#include <QWidget>

class ResultingChartofMetrics : public QWidget
{
    Q_OBJECT
public:
    explicit ResultingChartofMetrics (const QString &prjid, QWidget *parent = nullptr);
};

#endif // RESULTINGCHARTOFMETRICS_H
