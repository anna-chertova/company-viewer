/*
 * (c) Anna Chertova 2020
 * This class manages loading data from an xml file (implementation)
 */

#include "companydataloader.h"
#include <QFile>

CompanyDataLoader::CompanyDataLoader(CompanyDataModel *dataModel,
                                     QObject *parent)
    : QObject(parent), companyDataModel(dataModel)
{

}

void CompanyDataLoader::parseFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Text))
    {
        emit error(tr("Error opening file"),
                   tr("Could not open file %1").arg(fileName));
        return;
    }

    xmlReader.setDevice(&file);

    if (xmlReader.readNextStartElement()) {
       if (xmlReader.name() == QLatin1String("departments")) {
           parseDepartments();
       } else {
           xmlReader.raiseError(QObject::tr("The file %1 is not a departments file.").arg(fileName));
       }
   }
   if(xmlReader.error()) {
       emit error(tr("Error parsing file %1").arg(fileName), errorString());
   }

   file.close();
}

QString CompanyDataLoader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xmlReader.errorString())
            .arg(xmlReader.lineNumber())
            .arg(xmlReader.columnNumber());
}

void CompanyDataLoader::parseDepartments()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("departments"));

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QLatin1String("department"))
        {
            companyDataModel->addDepartment(parseDepartment());
            emit updateData();
        }
        else
        {
            xmlReader.skipCurrentElement();
        }
    }
}

Department CompanyDataLoader::parseDepartment()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("department"));

    Department d(xmlReader.attributes().value(
                     QStringLiteral("name")).toString());

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QLatin1String("employments"))
            d.employees = parseEmployments();
        else
            xmlReader.skipCurrentElement();
    }

    return d;
}

std::vector<Employee> CompanyDataLoader::parseEmployments()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("employments"));

    std::vector<Employee> employees;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QLatin1String("employment")) {
            Employee emp = parseEmployment();
            employees.push_back(emp);
        }
        else {
            xmlReader.skipCurrentElement();
        }
    }
    return employees;
}

Employee CompanyDataLoader::parseEmployment()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("employment"));

    Employee emp;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QLatin1String("surname"))
            emp.surname = parseSurname();
        else if (xmlReader.name() == QLatin1String("name"))
            emp.name = parseName();
        else if (xmlReader.name() == QLatin1String("middleName"))
            emp.middlename = parseMiddleName();
        else if (xmlReader.name() == QLatin1String("function"))
            emp.position = parseFunction();
        else if (xmlReader.name() == QLatin1String("salary"))
            emp.salary = parseSalary();
        else
            xmlReader.skipCurrentElement();
    }

    return emp;
}

QString CompanyDataLoader::parseSurname()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("surname"));
    return xmlReader.readElementText();
}

QString CompanyDataLoader::parseName()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("name"));
    return xmlReader.readElementText();
}

QString CompanyDataLoader::parseMiddleName()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("middleName"));
    return xmlReader.readElementText();
}

QString CompanyDataLoader::parseFunction()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("function"));
    return xmlReader.readElementText();
}

int CompanyDataLoader::parseSalary()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("salary"));
    return xmlReader.readElementText().toInt();
}
