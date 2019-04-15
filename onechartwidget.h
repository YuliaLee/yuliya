#ifndef ONECHARTWIDGET_H
#define ONECHARTWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

class OneChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OneChartWidget (const QString &prjid, QWidget *parent = nullptr);
    virtual ~OneChartWidget ();
};

#endif // ONECHARTWIDGET_H
