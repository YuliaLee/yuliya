#ifndef FAULTTOLERANCEMETRICS_H
#define FAULTTOLERANCEMETRICS_H

#include <QWidget>

class FaultToleranceMetrics : public QWidget
{
    Q_OBJECT
public:
    explicit FaultToleranceMetrics (const QString &prjid, QWidget *parent = nullptr);
    ~FaultToleranceMetrics ();
};

#endif // FAULTTOLERANCEMETRICS_H
