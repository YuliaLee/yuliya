#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
    class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog (QWidget *parent = nullptr);
    ~ConnectionDialog ();

protected slots:
    //!
    //! \brief Обработчик изменения текста в поле ввода
    //! \param value Новое значение текста
    //!
    void slotTextChanged (const QString &value);

    //! Обработчик кнопки "Подключиться"
    void slotConnectionClicked ();

private:
    void readSettings ();
    void writeSettings ();

    Ui::ConnectionDialog *ui;
};

#endif // CONNECTIONDIALOG_H
