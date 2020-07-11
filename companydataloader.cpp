#include "companydataloader.h"
#include <QXmlStreamReader>

CompanyDataLoader::CompanyDataLoader(QObject *parent) : QObject(parent)
{

}

bool CompanyDataLoader::parseFile(QFile *file)
{
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(file);
    xmlReader.readNext();
    while (!xmlReader.atEnd())
    {
        if (xmlReader.isStartElement())
        {
            if(xmlReader.name() == "department") { // found department
                // read attributes
                foreach(const QXmlStreamAttribute attr, xmlReader.attributes()) {
                    QString attributeName = attr.name().toString();
                    if(attributeName == "name") {
                        emit newDepartment(attributeName);
                    }
                }
            }
            else if(xmlReader.name() == "employment") { // found employee

            }
        }
        xmlReader.readNext();
    }

    return true;
}
