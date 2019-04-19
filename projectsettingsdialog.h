#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class ProjectSettingsDialog;
}

class ProjectSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectSettingsDialog (const QString &prjid, QWidget *parent = nullptr);
    ~ProjectSettingsDialog();

protected slots:
    void readSettings ();
    void writeSettings ();

private:
    Ui::ProjectSettingsDialog *ui;
    QString _prjid;
};

#endif // PROJECTSETTINGSDIALOG_H
