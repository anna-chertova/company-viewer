/*
 * (c) Anna Chertova 2020
 * This class manages loading data from an xml file
 */

#ifndef COMPANYDATALOADER_H
#define COMPANYDATALOADER_H

#include "companydatamodel.h"
#include <QObject>
#include <QXmlStreamReader>

class CompanyDataLoader : public QObject
{    
    Q_OBJECT

public:

    explicit CompanyDataLoader(CompanyDataModel *dataModel,
                               QObject *parent = nullptr);

public slots:

    // parse specified xml file
    void parseFile(const QString &fileName);    

signals:

    void updateData();
    void error(const QString &name, const QString &text);

private:

    // forms a human-readable error string from
    // xml parser errors
    QString errorString() const;
    // parses departments list
    void parseDepartments();
    // parses single department and returns a Department structure
    Department parseDepartment();
    // parses employees list and returns a vector of Employees structures
    std::vector<Employee> parseEmployments();
    // parse single employee and returns an Employee structure
    Employee parseEmployment();

    // parse employee data
    QString parseSurname();
    QString parseName();
    QString parseMiddleName();
    QString parseFunction();
    int parseSalary();

private:

    QXmlStreamReader xmlReader;
    CompanyDataModel *companyDataModel;
};

#endif // COMPANYDATALOADER_H
