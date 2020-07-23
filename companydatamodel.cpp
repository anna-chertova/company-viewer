#include "companydatamodel.h"

CompanyDataModel::CompanyDataModel(QObject *parent) : QAbstractItemModel(parent)
{

}

QModelIndex CompanyDataModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    // one of root indices is requested
    //if (!parent.isValid()) {
        Q_ASSERT(static_cast<int>(departments.size()) > row);
        return createIndex(row, column, const_cast<Department*>(&departments[row]));
    //}
    /*Department *department = static_cast<Department*>(parent.internalPointer());
    Q_ASSERT(department != NULL);
    Q_ASSERT(static_cast<int>(department->employees.size()) > row);
    return createIndex(row, column, &department->employees[row]);*/

    //return createIndex(row, column, static_cast<quintptr>(0));
}

QModelIndex CompanyDataModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }
    /*
    Employee* employee = static_cast<Employee*>(child.internalPointer());
    if (employee) { // if we request employee's parent (department)
        //int department_row = std::distance(departments.begin(), std::find(departments.begin(),departments.end(), *employee->department));
        auto colleagues = employee->department->employees;
        int employee_row = std::distance(colleagues.begin(), std::find(colleagues.begin(), colleagues.end(), *employee));
        return createIndex(employee_row, RamificationColumn, employee->department);
    }
    // departments have no parents
    */
    return QModelIndex();
}

int CompanyDataModel::rowCount(const QModelIndex &parent) const
{
    /*Q_ASSERT(checkIndex(parent,
                        QAbstractItemModel::CheckIndexOption::IndexIsValid |
                        QAbstractItemModel::CheckIndexOption::DoNotUseParent));*/

    if (!parent.isValid()) {
        return departments.size();
    }
    /*const Department *department = static_cast<const Department*>(parent.internalPointer());
    Q_ASSERT(department != 0);

    return department->employees.size();*/
    return 0;
}

int CompanyDataModel::columnCount(const QModelIndex &parent) const
{
    /*Q_ASSERT(checkIndex(parent,
                        QAbstractItemModel::CheckIndexOption::IndexIsValid |
                        QAbstractItemModel::CheckIndexOption::DoNotUseParent));*/

    return ColumnCount;
}

QVariant CompanyDataModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(checkIndex(index,
                        QAbstractItemModel::CheckIndexOption::IndexIsValid |
                        QAbstractItemModel::CheckIndexOption::DoNotUseParent));

    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    /// TODO: add logic to display employees (child items)    

    const Department *department = static_cast<Department*>(index.internalPointer());
    Q_ASSERT(department != 0);

    switch (index.column()) {
    case DepartmentName:
        return department->name;
    case DepartmentNumEmployees:
        return department->getNumEmployees();
    case EmployeeSalary:
        return department->getAvgSalary();
    default:
        break;
    }
    return QVariant();
}

bool CompanyDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(checkIndex(index,
                        QAbstractItemModel::CheckIndexOption::IndexIsValid |
                        QAbstractItemModel::CheckIndexOption::DoNotUseParent));

    if (!index.isValid()) {
        return false;
    }
    if (role != Qt::EditRole) {
        return false;
    }

    /// TODO: add logic to edit employees (child items)

    if (index.column() != DepartmentName) {
        return false;
    }

    QString newName = value.toString();
    Department *department = static_cast<Department*>(index.internalPointer());
    if(department != 0) {
        department->name = newName;
        return true;
    }
    return false;
}

QVariant CompanyDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList headers = {tr("Department"),
                                 tr("Num employees"),
                                 tr("Surname"),
                                 tr("Name"),
                                 tr("Middlename"),
                                 tr("Position"),
                                 tr("Salary")};
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section < headers.size()) {
        return headers[section];
    }
    return QVariant();
}

Qt::ItemFlags CompanyDataModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.isValid() && (index.column() == DepartmentName)) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

void CompanyDataModel::addDepartment(Department department)
{
    departments.push_back(department);
}

int CompanyDataModel::getNumDepartments() const
{
    return departments.size();
}

const Department& CompanyDataModel::getDepartment(int n) const
{
    return departments.at(n);
}

void CompanyDataModel::clear()
{
    removeRows(0, departments.size());
    departments.clear();
}
