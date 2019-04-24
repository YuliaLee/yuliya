#include "projectpassportdialog.h"
#include "ui_projectpassportdialog.h"
#include "redmineinstance.h"

#include <QSettings>

ProjectPassportDialog::ProjectPassportDialog (const QString &prjid, QWidget *parent)
    : QDialog (parent)
    , ui (new Ui::ProjectPassportDialog)
    , _prjid (prjid)
{
    ui->setupUi (this);
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[_prjid];
    if (!project.isNull ())
    {
        project->readSettings ();
        readSettings ();

        connect (ui->_editCodeLines, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
        connect (ui->_editExceptions, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
        connect (ui->_editIncorrectActions, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
        connect (ui->_editReferenceNumberOfErrors, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
        connect (ui->_editNeedTestCase, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
        connect (ui->_editCreatedTestCase, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
    }
}

ProjectPassportDialog::~ProjectPassportDialog ()
{
    delete ui;
}

void ProjectPassportDialog::readSettings ()
{
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[_prjid];
    if (project.isNull ())
        return;

    ui->_editCodeLines->setText (QString::number (project->_code_lines));
    ui->_editExceptions->setText (QString::number (project->_exceptions));
    ui->_editIncorrectActions->setText (QString::number (project->_incorrect_actions));
    ui->_editReferenceNumberOfErrors->setText (QString::number (project->_reference_number_of_error));
    ui->_editNeedTestCase->setText (QString::number (project->_need_test_case));
    ui->_editCreatedTestCase->setText (QString::number (project->_created_test_case));
}

void ProjectPassportDialog::writeSettings ()
{
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[_prjid];
    if (project.isNull ())
        return;

    project->_code_lines = ui->_editCodeLines->text ().trimmed ().toInt ();
    project->_exceptions = ui->_editExceptions->text ().trimmed ().toInt ();
    project->_incorrect_actions = ui->_editIncorrectActions->text ().trimmed ().toInt ();
    project->_reference_number_of_error = ui->_editReferenceNumberOfErrors->text ().trimmed ().toInt ();
    project->_need_test_case = ui->_editNeedTestCase->text ().trimmed ().toInt ();
    project->_created_test_case = ui->_editCreatedTestCase->text ().trimmed ().toInt ();

    project->writeSettings ();
}
