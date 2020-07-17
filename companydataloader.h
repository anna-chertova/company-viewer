/*
 * (c) Anna Chertova 2020
 * This class manages loading data from xml file
 */

#ifndef COMPANYDATALOADER_H
#define COMPANYDATALOADER_H

#include "companydata.h"
#include <QObject>
#include <QXmlStreamReader>

class CompanyDataLoader : public QObject
{    
    Q_OBJECT

public:

    explicit CompanyDataLoader(CompanyData *data, QObject *parent = nullptr);
    QString errorString() const;

public slots:

    void parseFile(const QString &fileName);    

signals:

    void updateData();
    void error(const QString &name, const QString &text);

private:

    void parseDepartments();
    Department parseDepartment();
    std::vector<Employee> parseEmployments();
    Employee parseEmployment();
    QString parseSurname();
    QString parseName();
    QString parseMiddleName();
    QString parseFunction();
    int parseSalary();

private:

    QXmlStreamReader xmlReader;
    CompanyData *companyData;
};

#endif // COMPANYDATALOADER_H
