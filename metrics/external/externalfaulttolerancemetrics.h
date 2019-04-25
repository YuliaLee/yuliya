#ifndef EXTERNALFAULTTOLERANCEMETRICS_H
#define EXTERNALFAULTTOLERANCEMETRICS_H

#include <QWidget>

namespace External
{

class FaultToleranceMetrics : public QWidget
{
    Q_OBJECT
public:
    explicit FaultToleranceMetrics (const QString &prjid, QWidget *parent = nullptr);
};

}

#endif // EXTERNALFAULTTOLERANCEMETRICS_H
