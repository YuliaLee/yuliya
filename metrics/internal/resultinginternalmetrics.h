#ifndef RESULTINGINTERNALMETRICSWIDGET_H
#define RESULTINGINTERNALMETRICSWIDGET_H

#include <QWidget>

class ResultingInternalMetricsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultingInternalMetricsWidget (const QString &prjid, QWidget *parent = nullptr);

protected slots:
    void slotMarkerClicked ();
};

#endif // RESULTINGINTERNALMETRICSWIDGET_H
