#ifndef RECOVERABILITYMETRICS_H
#define RECOVERABILITYMETRICS_H

#include <QWidget>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

//!
//! \brief Внутренняя метрика восстанавливаемости
//!
class RecoverabilityMetrics : public QWidget
{
    Q_OBJECT
public:
    explicit RecoverabilityMetrics (const QString &prjid, QWidget *parent = nullptr);
    void resizeEvent (QResizeEvent *event);

protected:
    int _maxw = 0;
    int _maxh = 0;
    QChart *_chart = nullptr;
};

#endif // RECOVERABILITYMETRICS_H
