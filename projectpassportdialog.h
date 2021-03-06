#ifndef PROJECTPASSPORTDIALOG_H
#define PROJECTPASSPORTDIALOG_H

#include <QDialog>

namespace Ui {
    class ProjectPassportDialog;
}

//!
//! \brief Диалоговое окно с паспортом проекта.
//! Паспорт объекта сохраняется через QSettings в файл
//! конфига.
//!
class ProjectPassportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectPassportDialog (const QString &prjid, QWidget *parent = nullptr);
    ~ProjectPassportDialog();

protected slots:
    void readSettings ();
    void writeSettings ();

private:
    Ui::ProjectPassportDialog *ui;
    QString _prjid;
};

#endif // PROJECTPASSPORTDIALOG_H
