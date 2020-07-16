/*
 * (c) Anna Chertova 2020
 * This class manages company data as a separate entity
 */

#ifndef COMPANYDATA_H
#define COMPANYDATA_H

#include "department.h"
#include <list>
#include <QObject>
#include <QStandardItemModel>
#include <QString>

/// TODO: shouldn't CompanyData be a implementation of QAbstractItemDataModel?
class CompanyData: public QObject
{
    Q_OBJECT

public:

    explicit CompanyData(QObject *parent = nullptr);
    QAbstractItemModel *getModel();

public slots:

    void addDepartment(const Department &department);

private:

    std::list<Department> departments;
    QStandardItemModel standardModel;
};

#endif // COMPANYDATA_H
