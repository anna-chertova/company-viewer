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

        }
        xmlReader.readNext();
    }

    return true;
}
