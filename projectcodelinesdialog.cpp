#include "projectcodelinesdialog.h"
#include "ui_projectcodelinesdialog.h"
#include "redmineinstance.h"

ProjectCodeLinesDialog::ProjectCodeLinesDialog (const QString &prjid, QWidget *parent)
    : QDialog(parent)
    , ui (new Ui::ProjectCodeLinesDialog)
    , _prjid (prjid)
{
    ui->setupUi(this);

    connect (ui->_btnCancel, SIGNAL(clicked(bool)), SLOT(slotCancel()));
    connect (ui->_btnSave, SIGNAL(clicked(bool)), SLOT(slotSave()));

    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[_prjid];
    if (project && RedmineInstance::instance ().loadIssues (_prjid))
    {
        int errors = 0;
        for (int i = 0; i < project->_issues.size (); ++i)
        {
            if (project->_issues[i]->_tracker_id == "1")
                errors++;
        }

        ui->_editErrors->setText (QString::number (errors));
    }

    if (project && RedmineInstance::instance ().loadIssues (_prjid))
    {
        int test_cases = 0;
        for (int i = 0; i < project->_issues.size (); ++i)
        {
            if (project->_issues[i]->_tracker_id == "5" &&
                    (project->_issues[i]->_status_id =="11" ||
                     project->_issues[i]->_status_id =="12" ))
                test_cases++;
        }

        ui->_editTestCases->setText (QString::number (test_cases));
    }

}

ProjectCodeLinesDialog::~ProjectCodeLinesDialog ()
{
    delete ui;
}

void ProjectCodeLinesDialog::slotCancel () {
    reject ();
}

void ProjectCodeLinesDialog::slotSave ()
{
    setProperty ("code_lines", ui->_editCodeLines->text ());
    setProperty ("errors", ui->_editErrors->text ());
    setProperty ("test_cases", ui->_editTestCases->text ());
    accept ();
}
