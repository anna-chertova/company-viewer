/*
 * (c) Anna Chertova 2020
 * Commands for undo/redo functionality
 */

#include "commands.h"

ChangeDataCommand::ChangeDataCommand(int dRow, int eRow, int col,
                                     const QVariant &oldVal,
                                     const QVariant &newVal,
                                     CompanyDataModel *model)
    : QUndoCommand(), departmentRow(dRow), employeeRow(eRow),
      column(col), newValue(newVal), oldValue(oldVal), dataModel(model)
{
    setText("Change item data");
}

void ChangeDataCommand::undo()
{
    Q_ASSERT(departmentRow >=0 );
    Q_ASSERT(departmentRow < static_cast<int>(dataModel->departmentItems.size()));
    DataItem *departmentItem = dataModel->departmentItems.at(departmentRow);
    if (employeeRow >=0)
    {
        Q_ASSERT(employeeRow < departmentItem->childCount());
        DataItem *employeeItem = departmentItem->child(employeeRow);
        employeeItem->setData(column, oldValue);
        // if employee salary has changed then recalculate avg salary for current department
        if(column == CompanyDataModel::EmployeeSalary)
            dataModel->updateDepartmentData(departmentItem);
    }
    else
    {
        departmentItem->setData(column, oldValue);
    }
}

void ChangeDataCommand::redo()
{
    Q_ASSERT(departmentRow >=0 );
    Q_ASSERT(departmentRow < static_cast<int>(dataModel->departmentItems.size()));
    DataItem *departmentItem = dataModel->departmentItems.at(departmentRow);
    if (employeeRow >=0)
    {
        Q_ASSERT(employeeRow < departmentItem->childCount());
        DataItem *employeeItem = departmentItem->child(employeeRow);
        employeeItem->setData(column, newValue);
        // if employee salary has changed then recalculate avg salary for current department
        if(column == CompanyDataModel::EmployeeSalary)
            dataModel->updateDepartmentData(departmentItem);
    }
    else
    {
        departmentItem->setData(column, newValue);
    }
}

/*************************************************************************************************/

AddDepartmentCommand::AddDepartmentCommand(int pos, int n, CompanyDataModel *model)
    : QUndoCommand(), position(pos), num(n), dataModel(model)
{
    if (n == 1)
        setText("Add Department item");
    else
        setText("Add Department items");
}

void AddDepartmentCommand::undo()
{
    // remove all employees inside departments being removed
    for(int i = 0; i < num; ++i) {
        DataItem *departmentItem = dataModel->departmentItems.at(position + i);
        int numEmployees = departmentItem->childCount();
        dataModel->beginRemoveRows(QModelIndex(), 0, numEmployees - 1);
        departmentItem->removeChildren(0, numEmployees);
        dataModel->endRemoveRows();
    }

    // remove departments
    dataModel->beginRemoveRows(QModelIndex(), position, position + num - 1);
    for(int i = 0; i < num; ++i) {
        delete dataModel->departmentItems.at(position + i);
    }
    dataModel->departmentItems.erase(
                dataModel->departmentItems.begin() + position,
                dataModel->departmentItems.begin() + position + num);
    dataModel->endRemoveRows();
}

void AddDepartmentCommand::redo()
{
    // add department rows
    std::vector<DataItem*> newDepartments;
    for(int i = 0; i < num; ++i) {
        DataItem *departmentItem = dataModel->createEmptyDepartmentItem();
        departmentItem->setData(CompanyDataModel::DepartmentName, "[Enter name]");
        newDepartments.push_back(departmentItem);
    }

    dataModel->beginInsertRows(QModelIndex(), position, position + num - 1);
    dataModel->departmentItems.insert(
                dataModel->departmentItems.begin() + position,
                newDepartments.begin(),
                newDepartments.end());
    dataModel->endInsertRows();
}

/*************************************************************************************************/

AddEmployeeCommand::AddEmployeeCommand(int dRow, int pos, int n, CompanyDataModel *model)
    : QUndoCommand(), departmentRow(dRow), position(pos), num(n), dataModel(model)
{
    if (n == 1)
        setText("Add Employee item");
    else
        setText("Add Employee items");
}

void AddEmployeeCommand::undo()
{
    Q_ASSERT(departmentRow >= 0);
    Q_ASSERT(departmentRow < static_cast<int>(dataModel->departmentItems.size()));
    DataItem *parentItem = dataModel->departmentItems.at(departmentRow);
    QModelIndex parentIndex = dataModel->index(
                dataModel->getRowNumber(parentItem),
                0,
                QModelIndex());
    Q_ASSERT(parentIndex.isValid());

    dataModel->beginRemoveRows(parentIndex, position, position + num - 1);
    parentItem->removeChildren(position, num);
    dataModel->endRemoveRows();

    dataModel->updateDepartmentData(parentItem);
}

void AddEmployeeCommand::redo()
{
    Q_ASSERT(departmentRow >= 0);
    Q_ASSERT(departmentRow < static_cast<int>(dataModel->departmentItems.size()));
    DataItem *parentItem = dataModel->departmentItems.at(departmentRow);
    QModelIndex parentIndex = dataModel->index(
                dataModel->getRowNumber(parentItem),
                0,
                QModelIndex());
    Q_ASSERT(parentIndex.isValid());

    dataModel->beginInsertRows(parentIndex, position, position + num - 1);
    parentItem->insertChildren(position, num, CompanyDataModel::ColumnCount);
    dataModel->endInsertRows();

    for (int i = 0; i < num; ++i) {
        DataItem *child = parentItem->child(position + i);
        child->setData(CompanyDataModel::EmployeeSurname, "[Enter surname]");
        child->setData(CompanyDataModel::EmployeeName, "[Enter name]");
        child->setData(CompanyDataModel::EmployeeMiddleName, "[Enter middlename]");
        child->setData(CompanyDataModel::EmployeePosition, "[Enter position]");
        child->setData(CompanyDataModel::EmployeeSalary, "[Enter salary]");
    }

    dataModel->updateDepartmentData(parentItem);
}

/*************************************************************************************************/

DeleteDepartmentCommand::DeleteDepartmentCommand(int pos, int n, CompanyDataModel *model)
    : QUndoCommand(), position(pos), num(n), dataModel(model)
{
    // Save data being removed
    for (int i = 0; i < n; ++i)
    {
        departmentData.push_back(dataModel->createDepartment(
                                     dataModel->departmentItems.at(pos + i)));
    }

    if (n == 1)
        setText("Delete Department item");
    else
        setText("Delete Department items");
}

void DeleteDepartmentCommand::undo()
{
    // restore department rows
    std::vector<DataItem*> newDepartments;
    for(int i = 0; i < num; ++i) {
        DataItem *departmentItem = dataModel->createEmptyDepartmentItem();
        Q_ASSERT(i < static_cast<int>(departmentData.size()));
        dataModel->fillDepartmentItem(departmentItem, departmentData[i]);
        newDepartments.push_back(departmentItem);
    }
    dataModel->beginInsertRows(QModelIndex(), position, position + num - 1);
    dataModel->departmentItems.insert(
                dataModel->departmentItems.begin() + position,
                newDepartments.begin(),
                newDepartments.end());
    dataModel->endInsertRows();
}

void DeleteDepartmentCommand::redo()
{
    // remove all employees inside departments being removed
    for(int i = 0; i < num; ++i) {
        DataItem *departmentItem = dataModel->departmentItems.at(position + i);
        const QModelIndex departmentIndex = dataModel->index(position + i, 0, QModelIndex());
        int numEmployees = departmentItem->childCount();
        dataModel->beginRemoveRows(departmentIndex, 0, numEmployees - 1);
        departmentItem->removeChildren(0, numEmployees);
        dataModel->endRemoveRows();
    }

    // remove department
    dataModel->beginRemoveRows(QModelIndex(), position, position + num - 1);
    for(int i = 0; i < num; ++i) {
        delete dataModel->departmentItems.at(position + i);
    }
    dataModel->departmentItems.erase(
                dataModel->departmentItems.begin() + position,
                dataModel->departmentItems.begin() + position + num);
    dataModel->endRemoveRows();
}

/*************************************************************************************************/

DeleteEmployeeCommand::DeleteEmployeeCommand(int dRow, int pos, int n, CompanyDataModel *model)
    : QUndoCommand(), departmentRow(dRow), position(pos), num(n), dataModel(model)
{
    Q_ASSERT(departmentRow >= 0);
    Q_ASSERT(departmentRow < static_cast<int>(dataModel->departmentItems.size()));
    DataItem *parentItem = dataModel->departmentItems.at(departmentRow);
    // Save data being removed
    for(int i = 0; i < num; ++i) {
        employeeData.push_back(dataModel->createEmployee(parentItem->child(pos + i)));
    }

    if (n == 1)
        setText("Delete Employee item");
    else
        setText("Delete Employee items");
}

void DeleteEmployeeCommand::undo()
{
    Q_ASSERT(departmentRow >= 0);
    Q_ASSERT(departmentRow < static_cast<int>(dataModel->departmentItems.size()));
    DataItem *parentItem = dataModel->departmentItems.at(departmentRow);

    QModelIndex parentIndex = dataModel->index(
                dataModel->getRowNumber(parentItem),
                0,
                QModelIndex());
    Q_ASSERT(parentIndex.isValid());

    dataModel->beginInsertRows(parentIndex, position, position + num - 1);
    parentItem->insertChildren(position, num, CompanyDataModel::ColumnCount);
    dataModel->endInsertRows();

    for (int i = 0; i < num; ++i)
    {
        Q_ASSERT(i < static_cast<int>(employeeData.size()));
        DataItem *child = parentItem->child(position + i);
        dataModel->fillEmployeeItem(child, employeeData[i]);
    }

    dataModel->updateDepartmentData(parentItem);
}

void DeleteEmployeeCommand::redo()
{
    Q_ASSERT(departmentRow >= 0);
    Q_ASSERT(departmentRow < static_cast<int>(dataModel->departmentItems.size()));
    DataItem *parentItem = dataModel->departmentItems.at(departmentRow);

    QModelIndex parentIndex = dataModel->index(
                dataModel->getRowNumber(parentItem),
                0,
                QModelIndex());
    Q_ASSERT(parentIndex.isValid());

    dataModel->beginRemoveRows(parentIndex, position, position + num - 1);
    parentItem->removeChildren(position, num);
    dataModel->endRemoveRows();

    dataModel->updateDepartmentData(parentItem);
}
