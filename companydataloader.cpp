#include "companydataloader.h"

CompanyDataLoader::CompanyDataLoader(QObject *parent) : QObject(parent)
{

}

bool CompanyDataLoader::parseFile(QFile *file)
{
    xmlReader.setDevice(file);

    if (xmlReader.readNextStartElement()) {
       if (xmlReader.name() == QLatin1String("xml")) {
           parseXml();
       } else {
           xmlReader.raiseError(QObject::tr("The file is not an XML file."));
       }
   }

    return !xmlReader.error();
}


void CompanyDataLoader::parseXml()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("xml"));

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QLatin1String("departments"))
            parseDepartments();
        else
            xmlReader.skipCurrentElement();
    }
}

void CompanyDataLoader::parseDepartments()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("departments"));

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QLatin1String("department"))
        {
            emit newDepartment(parseDepartment());
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

std::list<Employee> CompanyDataLoader::parseEmployments()
{
    Q_ASSERT(xmlReader.isStartElement() &&
             xmlReader.name() == QLatin1String("employments"));

    std::list<Employee> employees;

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
