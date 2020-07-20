#ifndef COMPANYDATAMODEL_H
#define COMPANYDATAMODEL_H

#include "department.h"
#include <QAbstractItemModel>

class CompanyDataModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CompanyDataModel(QObject *parent = nullptr);

    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

signals:

private:

    enum Columns
    {
        RamificationColumn,
        DepartmentName = RamificationColumn,
        DepartmentNumEmployees,
        EmployeeSurname,
        EmployeeName,
        EmployeeMidlleName,
        EmployeePosition,
        EmployeeSalary,
        ColumnCount
    };

    std::vector<Department> departments;

};

#endif // COMPANYDATAMODEL_H
