#ifndef RELIABILITYCOMPLIANCEMETRICS_H
#define RELIABILITYCOMPLIANCEMETRICS_H

#include <QWidget>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

//!
//! \brief Внутренняя метрика соответсвия надёжности
//!
class ReliabilityComplianceMetrics : public QWidget
{
    Q_OBJECT
public:
    explicit ReliabilityComplianceMetrics (const QString &prjid, QWidget *parent = nullptr);

protected:
    void resizeEvent (QResizeEvent *event);

    int _maxw = 0;
    int _maxh = 0;
    QChart *_chart = nullptr;
};

#endif // RELIABILITYCOMPLIANCEMETRICS_H
