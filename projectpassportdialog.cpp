#include "projectpassportdialog.h"
#include "ui_projectpassportdialog.h"
#include "redmineinstance.h"

#include <QSettings>
#include <QDebug>

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

        connect (ui->_editExceptions, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
        connect (ui->_editIncorrectActions, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
        connect (ui->_editReferenceNumberOfErrors, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
        connect (ui->_editNeedTestCase, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));
        connect (ui->_editThreshold, SIGNAL(textChanged(QString)),
                 SLOT(writeSettings()));

        auto dv = new QDoubleValidator (0.0, 5.0, 2); // [0, 1] with 2 decimals of precision
        ui->_editThreshold->setValidator (dv);
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

//--------------------------Вывод общего числа исключений
    if (project && RedmineInstance::instance ().loadIssues (_prjid))
    {
        int exceptions = 0;
        for (int i = 0; i < project->_issues.size (); ++i)
        {
            if (project->_issues[i]->_tracker_id == "4" &&           //функционал
               (project->_issues[i]->_category_id == "31" ||
                project->_issues[i]->_category_id == "32" ||
                project->_issues[i]->_category_id == "33" ||
                project->_issues[i]->_category_id == "36"))          //категория exception))
                    exceptions++;
        }

        ui->_editExceptions->setText (QString::number (exceptions));
    }

//--------------------------Вывод общего числа некорректных действий
    if (project && RedmineInstance::instance ().loadIssues (_prjid))
    {
        int incorrect_actions = 0;
        for (int i = 0; i < project->_issues.size (); ++i)
        {
            if (project->_issues[i]->_tracker_id == "4" &&           //функционал
                    (project->_issues[i]->_category_id == "37" ||
                     project->_issues[i]->_category_id == "38" ||
                     project->_issues[i]->_category_id == "39" ||
                     project->_issues[i]->_category_id == "47" ||
                     project->_issues[i]->_category_id == "48" ||
                     project->_issues[i]->_category_id == "49" ||
                     project->_issues[i]->_category_id == "50" ||
                     project->_issues[i]->_category_id == "51" ||
                     project->_issues[i]->_category_id == "52" ||
                     project->_issues[i]->_category_id == "53"))      // категория Check test
                incorrect_actions++;
        }

        ui->_editIncorrectActions->setText (QString::number (incorrect_actions));
    }

//--------------------------Вывод/ввод планируемого числа ошибок
    ui->_editReferenceNumberOfErrors->setText (QString::number (project->_reference_number_of_error));


//--------------------------Вывод/ввод количества тест-кейсов, необходимых по тест-плану
    ui->_editNeedTestCase->setText (QString::number (project->_need_test_case));
//    if (project && RedmineInstance::instance ().loadIssues (_prjid))
//    {
//        int _need_test_case = 0;
//        for (int i = 0; i < project->_issues.size (); ++i)
//        {
//            if (project->_issues[i]->_tracker_id == "5")
//                _need_test_case++;
//        }

//        ui->_editNeedTestCase->setText (QString::number (_need_test_case));
//    }

    ui->_editThreshold->setText (QString::number (project->_threshold, 'f', 2));
}

void ProjectPassportDialog::writeSettings ()
{
    QSharedPointer<RedmineProject> project = RedmineInstance::instance ().projects ()[_prjid];
    if (project.isNull ())
        return;

    project->_exceptions = ui->_editExceptions->text ().trimmed ().toInt ();
    project->_incorrect_actions = ui->_editIncorrectActions->text ().trimmed ().toInt ();
    project->_reference_number_of_error = ui->_editReferenceNumberOfErrors->text ().trimmed ().toInt ();
    project->_need_test_case = ui->_editNeedTestCase->text ().trimmed ().toInt ();
    project->_threshold = ui->_editThreshold->text ().trimmed ().toDouble ();

    project->writeSettings ();
}
