#ifndef RECOVERABILITYMETRICS_H
#define RECOVERABILITYMETRICS_H

#include <QWidget>

class RecoverabilityMetrics : public QWidget
{
    Q_OBJECT
public:
    explicit RecoverabilityMetrics (const QString &prjid, QWidget *parent = nullptr);
};

#endif // RECOVERABILITYMETRICS_H
