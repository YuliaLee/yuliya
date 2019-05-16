#ifndef RESULTINGEXTERNALMETRICS_H
#define RESULTINGEXTERNALMETRICS_H

#include <QWidget>

class ResultingExternalMetricsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResultingExternalMetricsWidget (const QString &prjid, QWidget *parent = nullptr);
};

#endif // RESULTINGEXTERNALMETRICS_H
