/*
 * (c) Anna Chertova 2020
 * This entity is responsible for saving CompanyData to an Xml file
 */

#ifndef COMPANYDATASAVER_H
#define COMPANYDATASAVER_H

#include "companydata.h"

#include <QObject>
#include <QXmlStreamWriter>

class CompanyDataSaver : public QObject
{
    Q_OBJECT
public:
    explicit CompanyDataSaver(CompanyData *data, QObject *parent = nullptr);

public slots:

    void saveFile(const QString &fileName);

signals:

    void error(const QString &name, const QString &text);

private:

    QXmlStreamWriter xmlWriter;
    CompanyData *companyData;
};

#endif // COMPANYDATASAVER_H
