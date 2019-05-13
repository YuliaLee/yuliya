#ifndef REDMINEINSTANCE_H
#define REDMINEINSTANCE_H

#include <QSharedPointer>
#include <QMap>

#include "redmineobjects.h"

//!
//! \brief Синглтон с объектами Redmine
//!
class RedmineInstance
{
public:
    static RedmineInstance &instance ();
    static void deleteInstance ();

    bool loadProjects ();
    const QMap<QString, QSharedPointer<RedmineProject> > &projects ();

    bool loadIssues (const QString &prjid);

    bool loadIssueStatuses ();
    const QMap<QString, QSharedPointer<RedmineIssueStatuses> > &statuses ();

    bool loadIssueCategories ();
    const QMap<QString, QSharedPointer<RedmineIssueCategories> > &categories ();

    bool loadTrackers ();
    const QMap<QString, QSharedPointer<RedmineTrackers> > &trackers ();

    bool loadAttachments ();
    const QMap<QString, QSharedPointer<RedmineAttachments> > &attachments ();

    const QList<RedmineProjectCodeMetrics> codeMetrics (const QString &prjid);



    //---------------------------------------- FaultToleranceMetrics Внутренние метрики устойчивости к ошибкам

    //------ Способность к предотвращению некоррестных действий

    //-- Количество разработанных задач для предотвращения некорректных действий
    int metric1A (const QString &prjid);

    //-- Планируемое количество некорректных действий
    int metric1B (const QString &prjid);

    //------ Коэффициент отказов

    //-- Число предотвращённых исключений
    int metric2A (const QString &prjid);

    //-- Число всех исключений
    int metric2B (const QString &prjid);

    //----------------------------------------- MaturityMetrics Внутренние метрики завершённости

    //------ Обнаружение ошибок

    //-- Число обнаруженных ошибок
    int metric3A (const QString &prjid);

    //-- Планируемое число ошибок
    int metric3B (const QString &prjid);

    //------ Устранение ошибок

    //-- Число решённых ошибок
    int metric4A (const QString &prjid);

    //-- Число всех ошибок
    int metric4B (const QString &prjid);

    //------ Адекватность теста

    //-- Подтверждённое кол-во тест-кейсов
    int metric5A (const QString &prjid);

    //-- Количество требуемых тест-кейсов
    int metric5B (const QString &prjid);

    //----------------- Recoverability Внутренние метрики восстанавливаемости

    //------ Восстанавливаемость

    //-- Число предотвращённых исключений
    int metric6A (const QString &prjid);

    //-- Планируемое количество исключений
    int metric6B (const QString &prjid);

    //----------------- ReliabilityCompliance Внутренняя метрика соответсвия надёжности

    //------ Внутренняя метрика соответсвия надёжности

    //-- Решенные задачи с вложениями
    int metric7A (const QString &prjid);

    //-- Общее количество задач с вложениями
    int metric7B (const QString &prjid);


    //----------------- Fault tolerance metrics Внешние метрики устойчивости к ошибкам

    //----  Коэффициент аварийных отказов

    //--Критические ошибки
    int metric8A (const QString &prjid);

    //--Все ошибки
    int metric8B (const QString &prjid);

    //----------------- Maturity metrics Внешние метрики завершенности

    //----  Разрешенние отказов

    //--Критические ошибки
    int metric9A (const QString &prjid);

    //--Все ошибки
    int metric9B (const QString &prjid);

    //----  Тестовое покрытие

    //--Запущенные тесты
    int metric10A (const QString &prjid);



    //---- Завершенность испытаний

    //---- Прошедшие тест-кейсы
    int metric11A (const QString &prjid);



    //---- Коэффициент отказов

    //---- Прошедшие для проверки исключений немедленные и срочные тест-кейсы
    int metric12A (const QString &prjid);

    //---- Запущенные для проверки исключений тест-кейсы
    int metric12B (const QString &prjid);


    //---- Предотвращение некорректных действий

    //---- Прошедшие для проверки некорректных действий немедленные и срочные тест-кейсы
    int metric13A (const QString &prjid);

    //---- Запущенные для проверки некорректных действий тест-кейсы
    int metric13B (const QString &prjid);

protected:

    RedmineInstance ();
    virtual ~RedmineInstance ();

    static RedmineInstance *_instance;

    QMap<QString, QSharedPointer<RedmineProject> > _projects;
    QMap<QString, QSharedPointer<RedmineIssueStatuses> > _statuses;
    QMap<QString, QSharedPointer<RedmineIssueCategories> > _categories;
    QMap<QString, QSharedPointer<RedmineTrackers> > _trackers;
    QMap<QString, QSharedPointer<RedmineAttachments> > _attachments;
};

#endif // REDMINEINSTANCE_H
