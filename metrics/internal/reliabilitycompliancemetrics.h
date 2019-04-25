#ifndef RELIABILITYCOMPLIANCEMETRICS_H
#define RELIABILITYCOMPLIANCEMETRICS_H

#include <QWidget>

class ReliabilityComplianceMetrics : public QWidget
{
    Q_OBJECT
public:
    explicit ReliabilityComplianceMetrics (const QString &prjid, QWidget *parent = nullptr);
};

#endif // RELIABILITYCOMPLIANCEMETRICS_H
