#ifndef PROJECTCODELINESDIALOG_H
#define PROJECTCODELINESDIALOG_H

#include <QDialog>

namespace Ui {
class ProjectCodeLinesDialog;
}

class ProjectCodeLinesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectCodeLinesDialog (const QString &prjid, QWidget *parent = nullptr);
    ~ProjectCodeLinesDialog ();

protected slots:
    void slotCancel ();
    void slotSave ();

private:
    Ui::ProjectCodeLinesDialog *ui;
    QString _prjid;
};

#endif // PROJECTCODELINESDIALOG_H
