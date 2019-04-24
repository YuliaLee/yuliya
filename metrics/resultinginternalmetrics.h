#ifndef RESULTINGINTERNALMETRICSWIDGET_H
#define RESULTINGINTERNALMETRICSWIDGET_H

#include <QWidget>

class ResultingInternalMetricsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultingInternalMetricsWidget (const QString &prjid, QWidget *parent = nullptr);
};

#endif // RESULTINGINTERNALMETRICSWIDGET_H
