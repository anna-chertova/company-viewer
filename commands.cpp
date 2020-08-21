/*
 * (c) Anna Chertova 2020
 * Commands for undo/redo functionality
 */

#include "commands.h"

ChangeDataCommand::ChangeDataCommand(DataItem *i,
                                     int col,
                                     const QVariant& val,
                                     CompanyDataModel *model)
    : QUndoCommand(), item(i), column(col), newValue(val), dataModel(model)
{
    oldValue = item->data(col);
    setText("Change item data");
}

void ChangeDataCommand::undo()
{
    item->setData(column, oldValue);

    // if employee salary has changed then recalculate avg salary for current department
    if(item->parent() && column == CompanyDataModel::EmployeeSalary)
        dataModel->updateDepartmentData(item->parent());
    }

void ChangeDataCommand::redo()
{
    item->setData(column, newValue);

    // if employee salary has changed then recalculate avg salary for current department
    if(item->parent() && column == CompanyDataModel::EmployeeSalary)
        dataModel->updateDepartmentData(item->parent());
}

/*************************************************************************************************/

AddDepartmentCommand::AddDepartmentCommand(int pos, int n, CompanyDataModel *model)
    : QUndoCommand(), position(pos), num(n), dataModel(model)
{
    setText("Add Department item");
}

void AddDepartmentCommand::undo()
{
    // remove all employees inside departments being removed
    for(int i = 0; i < num; ++i) {
        DataItem *departmentItem = dataModel->departmentItems.at(position + i);
        dataModel->beginRemoveRows(QModelIndex(), 0, num - 1);
        departmentItem->removeChildren(0, num);
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
        newDepartments.push_back(dataModel->createEmptyDepartment());
    }
    dataModel->beginInsertRows(QModelIndex(), position, position + num - 1);
    dataModel->departmentItems.insert(
                dataModel->departmentItems.begin() + position,
                newDepartments.begin(),
                newDepartments.end());
    dataModel->endInsertRows();
}

/*************************************************************************************************/

AddEmployeeCommand::AddEmployeeCommand(DataItem *parent, int pos, int n, CompanyDataModel *model)
    : QUndoCommand(), parentItem(parent), position(pos), num(n), dataModel(model)
{
    setText("Add Employee item");
}

void AddEmployeeCommand::undo()
{
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
    QModelIndex parentIndex = dataModel->index(
                dataModel->getRowNumber(parentItem),
                0,
                QModelIndex());
    Q_ASSERT(parentIndex.isValid());

    dataModel->beginInsertRows(parentIndex, position, position + num - 1);
    parentItem->insertChildren(position, num, CompanyDataModel::ColumnCount);
    dataModel->endInsertRows();

    dataModel->updateDepartmentData(parentItem);
}

/*************************************************************************************************/

DeleteDepartmentCommand::DeleteDepartmentCommand(int pos, int n, CompanyDataModel *model)
    : QUndoCommand(), position(pos), num(n), dataModel(model)
{
    setText("Delete Department item");
}

void DeleteDepartmentCommand::undo()
{
    // add department rows
    std::vector<DataItem*> newDepartments;
    for(int i = 0; i < num; ++i) {
        newDepartments.push_back(dataModel->createEmptyDepartment());
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
        dataModel->beginRemoveRows(QModelIndex(), 0, num - 1);
        departmentItem->removeChildren(0, num);
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

DeleteEmployeeCommand::DeleteEmployeeCommand(DataItem *parent, int pos, int n, CompanyDataModel *model)
    : QUndoCommand(), parentItem(parent), position(pos), num(n), dataModel(model)
{
    setText("Delete Employee item");
}

void DeleteEmployeeCommand::undo()
{
    QModelIndex parentIndex = dataModel->index(
                dataModel->getRowNumber(parentItem),
                0,
                QModelIndex());
    Q_ASSERT(parentIndex.isValid());

    dataModel->beginInsertRows(parentIndex, position, position + num - 1);
    parentItem->insertChildren(position, num, CompanyDataModel::ColumnCount);
    dataModel->endInsertRows();

    dataModel->updateDepartmentData(parentItem);
}

void DeleteEmployeeCommand::redo()
{
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
