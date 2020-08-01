/*
 * (c) Anna Chertova 2020
 * This entity is responsible for saving company data to an xml file
 */

#include "companydatasaver.h"
#include <QFile>

CompanyDataSaver::CompanyDataSaver(CompanyDataModel *data, QObject *parent)
    : QObject(parent), companyData(data)
{

}

void CompanyDataSaver::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Text))
    {
        emit error(tr("Error opening file"),
                   tr("Could not open file %1").arg(fileName));
        return;
    }

    xmlWriter.setDevice(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("departments");

    for(int i = 0; i < companyData->getNumDepartments(); ++i) {

        Department d = companyData->getDepartment(i);
        xmlWriter.writeStartElement("department");
        xmlWriter.writeAttribute("name", d.name);
        xmlWriter.writeStartElement("employments");

        for (int j = 0; j < static_cast<int>(d.employees.size()); ++j) {

            xmlWriter.writeStartElement("employment");
            Employee e = d.employees[j];

            if(!e.surname.isEmpty())
                xmlWriter.writeTextElement("surname",e.surname);
            if(!e.name.isEmpty())
                xmlWriter.writeTextElement("name",e.name);
            if(!e.middlename.isEmpty())
                xmlWriter.writeTextElement("middleName",e.middlename);
            if(!e.position.isEmpty())
                xmlWriter.writeTextElement("function",e.position);
            xmlWriter.writeTextElement("salary",QString::number(e.salary));
            xmlWriter.writeEndElement(); //employment
        }
        xmlWriter.writeEndElement(); // employments
        xmlWriter.writeEndElement(); // department
    }

    xmlWriter.writeEndElement(); // departments
    xmlWriter.writeEndDocument();
    file.close();
}
