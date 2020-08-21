/*
 * (c) Anna Chertova 2020
 * Hierarchical model implementation for company data
 */

#include "companydatamodel.h"
#include "commands.h"

CompanyDataModel::CompanyDataModel(QObject *parent): QAbstractItemModel(parent)
{
    undoStack = new QUndoStack(this);
}

CompanyDataModel::~CompanyDataModel()
{
    qDeleteAll(departmentItems);
    departmentItems.clear();
}

QModelIndex CompanyDataModel::index(int row,
                                    int column,
                                    const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    // one of root indices is requested (department)
    if (!parent.isValid()) {
        Q_ASSERT(row >= 0);
        Q_ASSERT(row < static_cast<int>(departmentItems.size()));
        return createIndex(row,
                           column,
                           const_cast<DataItem*>(departmentItems[row]));
    }

    // one of child indices is requested (employee)
    DataItem *departmentItem =
            static_cast<DataItem*>(parent.internalPointer());
    Q_ASSERT(departmentItem != nullptr);
    Q_ASSERT(row >= 0);
    Q_ASSERT(static_cast<int>(row < departmentItem->childCount()));
    return createIndex(row, column, departmentItem->child(row));
}

QModelIndex CompanyDataModel::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    // if we request employee's parent (department)
    DataItem *employeeItem = static_cast<DataItem*>(child.internalPointer());
    if (employeeItem) {
        DataItem *departmentItem = employeeItem->parent();
        if (departmentItem) {
            return createIndex(
                        getRowNumber(departmentItem),
                        0,
                        departmentItem);
        }
    }
    // departments have no parents    
    return QModelIndex();
}

int CompanyDataModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return departmentItems.size();
    }

    DataItem *departmentItem =
            static_cast<DataItem*>(parent.internalPointer());
    Q_ASSERT(departmentItem != nullptr);

    return departmentItem->childCount();
}

int CompanyDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
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

    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    const DataItem *item = static_cast<DataItem*>(index.internalPointer());
    Q_ASSERT(item != nullptr);
    return item->data(index.column());
}

bool CompanyDataModel::setData(const QModelIndex &index,
                               const QVariant &value,
                               int role)
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

    DataItem *item = static_cast<DataItem*>(index.internalPointer());
    Q_ASSERT(item != nullptr);
    Q_ASSERT(flags(index) && Qt::ItemIsEditable);

    undoStack->push(new ChangeDataCommand(item, index.column(), value, this));

    return true;
}

QVariant CompanyDataModel::headerData(int section,
                                      Qt::Orientation orientation,
                                      int role) const
{
    const QStringList headers = {tr("Department"),
                                 tr("Num employees"),
                                 tr("Surname"),
                                 tr("Name"),
                                 tr("Middlename"),
                                 tr("Position"),
                                 tr("Salary")};
    if (orientation == Qt::Horizontal &&
        role == Qt::DisplayRole &&
        section < headers.size()) {

        return headers[section];
    }
    return QVariant();
}

Qt::ItemFlags CompanyDataModel::flags(const QModelIndex &index) const
{    
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    DataItem *item = static_cast<DataItem*>(index.internalPointer());
    Q_ASSERT(item != nullptr);

    if (index.isValid()) {
        if ((!item->parent() && index.column() == DepartmentName) ||
            (item->parent() && (index.column() == EmployeeSurname ||
                                index.column() == EmployeeName ||
                                index.column() == EmployeeMiddleName ||
                                index.column() == EmployeePosition ||
                                index.column() == EmployeeSalary))) {
            flags |= Qt::ItemIsEditable;
        }
    }

    return flags;
}


bool CompanyDataModel::insertRows(int position,
                                  int rows,
                                  const QModelIndex &parent)
{
    if(!parent.isValid())
    {
        undoStack->push(new AddDepartmentCommand(position, rows, this));
        return true;
    }

    // add employee row
    DataItem *parentItem = static_cast<DataItem*>(parent.internalPointer());
    if (!parentItem)
       return false;

    undoStack->push(new AddEmployeeCommand(parentItem, position, rows, this));
    return true;
}

bool CompanyDataModel::removeRows(int position,
                                  int rows,
                                  const QModelIndex &parent)
{
    if(rows == 0) { // nothing to remove
        return false;
    }

    if(!parent.isValid()) { // department(s) is(are) removed
        undoStack->push(new DeleteDepartmentCommand(position, rows, this));
        return true;
    }

    // employee(s) is (are) removed
    DataItem *parentItem = static_cast<DataItem*>(parent.internalPointer());
    if (!parentItem) {
        return false;
    }

    undoStack->push(new DeleteEmployeeCommand(parentItem, position, rows, this));
    return true;
}

void CompanyDataModel::addDepartment(Department department)
{
    DataItem *departmentItem = createEmptyDepartment();
    departmentItem->setData(DepartmentName, department.name);

    for(int i = 0; i < static_cast<int>(department.employees.size()); ++i) {

        Employee employee = department.employees[i];

        departmentItem->insertChildren(
                    departmentItem->childCount(),
                    1,
                    ColumnCount);

        DataItem *employeeItem = departmentItem->child(
                    departmentItem->childCount() - 1);
        employeeItem->setData(DepartmentName, "");
        employeeItem->setData(DepartmentNumEmployees, "");
        employeeItem->setData(EmployeeSurname, employee.surname);
        employeeItem->setData(EmployeeName, employee.name);
        employeeItem->setData(EmployeeMiddleName, employee.middlename);
        employeeItem->setData(EmployeePosition, employee.position);
        employeeItem->setData(EmployeeSalary, employee.salary);
    }

    departmentItems.push_back(departmentItem);

    updateDepartmentData(departmentItem);
}

int CompanyDataModel::getNumDepartments() const
{
    return departmentItems.size();
}

Department CompanyDataModel::getDepartment(int n) const
{
    DataItem *item = departmentItems.at(n);

    Department department;
    department.name = item->data(DepartmentName).toString();

    for(int i = 0; i < item->childCount(); ++i) {
       Employee employee;
       DataItem *childItem = item->child(i);
       employee.surname = childItem->data(EmployeeSurname).toString();
       employee.name = childItem->data(EmployeeName).toString();
       employee.middlename = childItem->data(EmployeeMiddleName).toString();
       employee.position = childItem->data(EmployeePosition).toString();
       employee.salary = childItem->data(EmployeeSalary).toInt();
       department.employees.push_back(employee);
    }

    return department;
}

QUndoStack *CompanyDataModel::getUndoStack() const
{
    return undoStack;
}

void CompanyDataModel::clear()
{
    // clear company data
    removeRows(0, departmentItems.size());
}

DataItem *CompanyDataModel::createEmptyDepartment()
{
    // create department with default values
    std::vector<QVariant> departmentValues;
    departmentValues.push_back("");
    departmentValues.push_back(0);
    departmentValues.push_back("");
    departmentValues.push_back("");
    departmentValues.push_back("");
    departmentValues.push_back("");
    departmentValues.push_back(0);
    DataItem *departmentItem = new DataItem(departmentValues);
    return departmentItem;
}

void CompanyDataModel::updateDepartmentData(DataItem *departmentItem)
{
    int numEmployees = departmentItem->childCount();
    int avgSalary = 0;
    int sumSalary = 0;
    if (numEmployees > 0) {
        for (int i = 0; i < numEmployees; ++i) {
            DataItem *employee = departmentItem->child(i);
            Q_ASSERT(employee != nullptr);
            sumSalary += employee->data(EmployeeSalary).toInt();
        }

        avgSalary = std::round(static_cast<float>(sumSalary) /
                               static_cast<float>(numEmployees));
    }
    departmentItem->setData(DepartmentNumEmployees, numEmployees);
    departmentItem->setData(EmployeeSalary, avgSalary);
}

int CompanyDataModel::getRowNumber(DataItem *departmentItem) const
{
    Q_ASSERT(departmentItem->parent() == nullptr);
    auto offset = std::find(departmentItems.begin(),
                            departmentItems.end(),
                            departmentItem);
    return std::distance(departmentItems.begin(), offset);
}
