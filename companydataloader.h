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

signals:

    void newDepartment(Department &d);

private:

    void parseXml();
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
