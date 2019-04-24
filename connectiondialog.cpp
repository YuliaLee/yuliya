#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include <QSettings>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

ConnectionDialog::ConnectionDialog (QWidget *parent)
    : QDialog (parent)
    , ui (new Ui::ConnectionDialog)
{
    ui->setupUi (this);
    readSettings ();

    connect (ui->_editIp, SIGNAL(textChanged(QString)),
             SLOT(slotTextChanged(QString)));
    connect (ui->_editPort, SIGNAL(textChanged(QString)),
             SLOT(slotTextChanged(QString)));
    connect (ui->_editUser, SIGNAL(textChanged(QString)),
             SLOT(slotTextChanged(QString)));
    connect (ui->_editDbName, SIGNAL(textChanged(QString)),
             SLOT(slotTextChanged(QString)));

    connect (ui->_buttonConnect, SIGNAL(clicked(bool)),
             SLOT(slotConnectionClicked()));

    slotTextChanged (QString::null);
}

ConnectionDialog::~ConnectionDialog ()
{
    delete ui;
}

void ConnectionDialog::slotTextChanged (const QString &value)
{
    (void)value;

    if (ui->_editIp->text ().isEmpty ()) {

    } else {

    }

    if (ui->_editPort->text ().isEmpty ()) {

    } else {

    }

    if (ui->_editUser->text ().isEmpty ()) {

    } else {

    }

    if (ui->_editDbName->text ().isEmpty ()) {

    } else {

    }

    if (ui->_editIp->text ().isEmpty () ||
            ui->_editPort->text ().isEmpty () ||
            ui->_editUser->text ().isEmpty () ||
            ui->_editDbName->text ().isEmpty ())
        ui->_buttonConnect->setEnabled (false);
    else
        ui->_buttonConnect->setEnabled (true);
}

void ConnectionDialog::slotConnectionClicked ()
{
    QSqlDatabase db = QSqlDatabase::addDatabase ("QPSQL", "yuliya");
    db.setHostName (ui->_editIp->text ().trimmed ());
    db.setPort (ui->_editPort->text ().trimmed ().toInt ());
    db.setDatabaseName (ui->_editDbName->text ().trimmed ());
    db.setUserName (ui->_editUser->text ().trimmed ());
    db.setPassword (ui->_editPassword->text ().trimmed ());

    if (!db.open ())
    {
        qCritical () << "[ConnectionDialog][slotConnectionClicked] Connection error";
        qCritical () << "[ConnectionDialog][slotConnectionClicked]" << db.lastError ().databaseText ();
        qCritical () << "[ConnectionDialog][slotConnectionClicked]" << db.lastError ().driverText ();
        qCritical () << "[ConnectionDialog][slotConnectionClicked]" << db.lastError ().text ();

        writeSettings ();
        reject ();
        return;
    }

    writeSettings ();
    accept ();
}

void ConnectionDialog::readSettings ()
{
    QSettings settings ("Inteltech", "yuliya");
    settings.beginGroup ("last_connection");
    ui->_editIp->setText (settings.value ("ip").toString ());
    ui->_editPort->setText (settings.value ("port").toString ());
    ui->_editDbName->setText (settings.value ("db_name").toString ());
    ui->_editUser->setText (settings.value ("user").toString ());
    ui->_editPassword->setText (settings.value ("password").toString ());
    settings.endGroup ();
}

void ConnectionDialog::writeSettings ()
{
    QSettings settings ("Inteltech", "yuliya");
    settings.beginGroup ("last_connection");
    settings.setValue ("ip", ui->_editIp->text ().trimmed ());
    settings.setValue ("port", ui->_editPort->text ().trimmed ());
    settings.setValue ("db_name", ui->_editDbName->text ().trimmed ());
    settings.setValue ("user", ui->_editUser->text ().trimmed ());
    settings.setValue ("password", ui->_editPassword->text ().trimmed ());
    settings.endGroup ();
}
