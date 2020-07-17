#include "companydata.h"

CompanyData::CompanyData(QObject *)
{
    standardModel.setHorizontalHeaderLabels({tr("Department"),
                                             tr("Num employees"),
                                             tr("Surname"),
                                             tr("Name"),
                                             tr("Middlename"),
                                             tr("Position"),
                                             tr("Salary")});
    root = standardModel.invisibleRootItem();
}

QAbstractItemModel *CompanyData::getModel()
{
    return &standardModel;
}

void CompanyData::clear()
{
    departments.clear();
    int row_count = standardModel.rowCount();
    standardModel.removeRows(0, row_count);
}

int CompanyData::getNumDepartments() const
{
    return departments.size();
}

const Department& CompanyData::getDepartment(int n) const
{
    return departments.at(n);
}

void CompanyData::addDepartment(Department department)
{
    departments.push_back(department);

    QList<QStandardItem*> departmentRow = {        
        new QStandardItem(department.name),
        new QStandardItem(QString::number(department.getNumEmployees())),        
        new QStandardItem(),
        new QStandardItem(),
        new QStandardItem(),
        new QStandardItem(),
        new QStandardItem(QString::number(department.getAvgSalary()))};
    root->appendRow(departmentRow);

    for (auto emp : department.employees) {
        departmentRow.first()->appendRow(
        {new QStandardItem(),
         new QStandardItem(),
         new QStandardItem(emp.surname),
         new QStandardItem(emp.name),
         new QStandardItem(emp.middlename),
         new QStandardItem(emp.position),
         new QStandardItem(QString::number(emp.salary))});
    }
}
