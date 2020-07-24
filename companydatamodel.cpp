#include "companydatamodel.h"

CompanyDataModel::CompanyDataModel(QObject *parent) : QAbstractItemModel(parent)
{

}

CompanyDataModel::~CompanyDataModel()
{
    qDeleteAll(departmentItems);
}

QModelIndex CompanyDataModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    // one of root indices is requested
    //if (!parent.isValid()) {
        Q_ASSERT(static_cast<int>(departmentItems.size()) > row);
        return createIndex(row, column, const_cast<DataItem*>(departmentItems[row]));
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

    // if we request employee's parent (department)
    /*Employee* employee = static_cast<Employee*>(child.internalPointer());
    if (employee) {
        //int department_row = std::distance(departments.begin(), std::find(departments.begin(),departments.end(), *employee->department));
        auto colleagues = employee->department->employees;
        int employee_row = std::distance(colleagues.begin(), std::find(colleagues.begin(), colleagues.end(), *employee));
        return createIndex(employee_row, RamificationColumn, employee->department);
    }*/
    // departments have no parents    
    return QModelIndex();
}

int CompanyDataModel::rowCount(const QModelIndex &parent) const
{
    /*Q_ASSERT(checkIndex(parent,
                        QAbstractItemModel::CheckIndexOption::IndexIsValid |
                        QAbstractItemModel::CheckIndexOption::DoNotUseParent));*/

    if (!parent.isValid()) {
        return departmentItems.size();
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

    const DataItem *departmentItem = static_cast<DataItem*>(index.internalPointer());
    Q_ASSERT(departmentItem != nullptr);
    return departmentItem->data(index.column());
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
    DataItem *departmentItem = static_cast<DataItem*>(index.internalPointer());
    if(departmentItem != nullptr) {
        departmentItem->setData(DepartmentName, newName);
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
    std::vector<QVariant> departmentValues;
    departmentValues.push_back(department.name);
    departmentValues.push_back(department.getNumEmployees());
    departmentValues.push_back("");
    departmentValues.push_back("");
    departmentValues.push_back("");
    departmentValues.push_back("");
    departmentValues.push_back(department.getAvgSalary());
    DataItem *departmentItem = new DataItem(departmentValues);

    for(int i = 0; i < static_cast<int>(department.employees.size()); ++i) {
        Employee employee = department.employees[i];
        if (departmentItem->insertChildren(departmentItem->childCount(), 1, ColumnCount)) {
            DataItem *employeeItem = departmentItem->child(departmentItem->childCount() - 1);
            employeeItem->setData(DepartmentName, "");
            employeeItem->setData(DepartmentNumEmployees, 0);
            employeeItem->setData(EmployeeSurname, employee.surname);
            employeeItem->setData(EmployeeName, employee.name);
            employeeItem->setData(EmployeeMidlleName, employee.middlename);
            employeeItem->setData(EmployeePosition, employee.position);
            employeeItem->setData(EmployeeSalary, employee.salary);
        }
        else {
            /// TODO: handle error adding employee
        }
    }

    departmentItems.push_back(departmentItem);
}

int CompanyDataModel::getNumDepartments() const
{
    return departmentItems.size();
}

Department CompanyDataModel::getDepartment(int n) const
{
    /// TODO: can we avoid this conversion here?

    DataItem *item = departmentItems.at(n);

    Department department;
    department.name = item->data(DepartmentName).toString();

    for(int i = 0; i < item->childCount(); ++i) {
       Employee employee(&department);
       DataItem *childItem = item->child(i);
       employee.surname = childItem->data(EmployeeSurname).toString();
       employee.name = childItem->data(EmployeeName).toString();
       employee.middlename = childItem->data(EmployeeMidlleName).toString();
       employee.position = childItem->data(EmployeePosition).toString();
       employee.salary = childItem->data(EmployeeSalary).toInt();
       department.employees.push_back(employee);
    }

    return department;
}

void CompanyDataModel::clear()
{
    /// TODO: check here to avoid memory leaks!
    removeRows(0, departmentItems.size());
    qDeleteAll(departmentItems);
    departmentItems.clear();
}

