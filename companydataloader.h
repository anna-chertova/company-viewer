/*
 * (c) Anna Chertova 2020
 * This class manages loading data from xml file
 */

#ifndef COMPANYDATALOADER_H
#define COMPANYDATALOADER_H

#include "department.h"
#include <QFile>
#include <QObject>
#include <QString>
#include <QXmlStreamReader>

class CompanyDataLoader : public QObject
{    
    Q_OBJECT

public:

    explicit CompanyDataLoader(QObject *parent = nullptr);

    bool parseFile(QFile *file);
    QString errorString() const;

signals:

    void newDepartment(Department d);
    void error(const QString &name, const QString &text);

private:

    void parseDepartments();
    Department parseDepartment();
    std::list<Employee> parseEmployments();
    Employee parseEmployment();
    QString parseSurname();
    QString parseName();
    QString parseMiddleName();
    QString parseFunction();
    int parseSalary();

private:

    QXmlStreamReader xmlReader;

};

#endif // COMPANYDATALOADER_H
