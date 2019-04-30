#ifndef RESULTINGEXTERNALMETRICS_H
#define RESULTINGEXTERNALMETRICS_H

#include <QWidget>

namespace External
{

}
class ResultingExternalMetricsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResultingExternalMetricsWidget (const QString &prjid, QWidget *parent = nullptr);
};

class ResultingExternalMetricsWidget2 : public QWidget
{
    Q_OBJECT
public:
    explicit ResultingExternalMetricsWidget2 (const QString &prjid, QWidget *parent = nullptr);
};


#endif // RESULTINGEXTERNALMETRICS_H
