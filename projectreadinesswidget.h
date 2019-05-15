#ifndef PROJECTREADINESSWIDGET_H
#define PROJECTREADINESSWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class ProjectReadinessWidget;
}

//!
//! \brief Оценка степени реализации требований к СПО
//!
class ProjectReadinessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectReadinessWidget (const QString &prjid, QWidget *parent = nullptr);
    ~ProjectReadinessWidget ();

private:
    Ui::ProjectReadinessWidget *ui;
    QStandardItemModel *_model = nullptr;

    QString _prjid;
};

#endif // PROJECTREADINESSWIDGET_H
