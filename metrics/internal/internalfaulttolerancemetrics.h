#ifndef FAULTTOLERANCEMETRICS_H
#define FAULTTOLERANCEMETRICS_H

#include <QWidget>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

namespace Internal
{

//!
//! \brief Внутренние метрики устойчивости к ошибкам
//!
class FaultToleranceMetrics : public QWidget
{
    Q_OBJECT
public:
    explicit FaultToleranceMetrics (const QString &prjid, QWidget *parent = nullptr);
    ~FaultToleranceMetrics ();

protected:
    void resizeEvent (QResizeEvent *event);

    int _maxw0 = 0;
    int _maxh0 = 0;
    QChart *_chart0 = nullptr;

    int _maxw1 = 0;
    int _maxh1 = 0;
    QChart *_chart1 = nullptr;
};

}

#endif // FAULTTOLERANCEMETRICS_H
