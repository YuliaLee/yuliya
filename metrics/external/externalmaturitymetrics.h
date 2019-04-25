#ifndef EXTERNAL_MATURITYMETRICSWIDGET_H
#define EXTERNAL_MATURITYMETRICSWIDGET_H

#include <QWidget>

namespace External
{

class MaturityMetricsWidget1 : public QWidget
{
    Q_OBJECT
public:
    explicit MaturityMetricsWidget1 (const QString &prjid, QWidget *parent = nullptr);
};

class MaturityMetricsWidget2 : public QWidget
{
    Q_OBJECT
public:
    explicit MaturityMetricsWidget2 (const QString &prjid, QWidget *parent = nullptr);
};

}

#endif //EXTERNAL_MATURITYMETRICSWIDGET_H
