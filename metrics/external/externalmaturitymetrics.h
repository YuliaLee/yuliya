#ifndef EXTERNAL_MATURITYMETRICSWIDGET_H
#define EXTERNAL_MATURITYMETRICSWIDGET_H

#include <QWidget>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

namespace External
{

//!
//! \brief Внешние метрики завершённости
//!
class MaturityMetricsWidget1 : public QWidget
{
    Q_OBJECT
public:
    explicit MaturityMetricsWidget1 (const QString &prjid, QWidget *parent = nullptr);
};

//!
//! \brief Внешние метрики завершённости
//!
class MaturityMetricsWidget2 : public QWidget
{
    Q_OBJECT
public:
    explicit MaturityMetricsWidget2 (const QString &prjid, QWidget *parent = nullptr);

protected:
    void resizeEvent (QResizeEvent *event);

    int _maxw0 = 0;
    int _maxh0 = 0;
    QChart *_chart0 = nullptr;

    int _maxw1 = 0;
    int _maxh1 = 0;
    QChart *_chart1 = nullptr;
};

//!
//! \brief Внешние метрики завершённости
//!
class MaturityMetricsWidget3 : public QWidget
{
    Q_OBJECT
public:
    explicit MaturityMetricsWidget3 (const QString &prjid, QWidget *parent = nullptr);

protected:
    void resizeEvent (QResizeEvent *event);

    int _maxw0 = 0;
    int _maxh0 = 0;
    QChart *_chart0 = nullptr;

    int _maxw1 = 0;
    int _maxh1 = 0;
    QChart *_chart1 = nullptr;

    int _maxw2 = 0;
    int _maxh2 = 0;
    QChart *_chart2 = nullptr;
};

}

#endif //EXTERNAL_MATURITYMETRICSWIDGET_H
