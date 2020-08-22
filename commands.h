/*
 * (c) Anna Chertova 2020
 * Commands for undo/redo functionality
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "companydatamodel.h"
#include "dataitem.h"
#include "department.h"

class ChangeDataCommand: public QUndoCommand
{
public:
    ChangeDataCommand(DataItem *i, int col, const QVariant &val, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    DataItem *item;
    int column;
    QVariant newValue, oldValue;
    CompanyDataModel *dataModel;
};

class AddDepartmentCommand : public QUndoCommand
{
public:
    AddDepartmentCommand(int pos, int n, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    int position;
    int num;
    std::vector<Department> departmentData;
    CompanyDataModel *dataModel;
};

class AddEmployeeCommand : public QUndoCommand
{
public:
    AddEmployeeCommand(DataItem *parent, int pos, int n, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    DataItem *parentItem;
    int position;
    int num;
    std::vector<Employee> employeeData;
    CompanyDataModel *dataModel;
};

class DeleteDepartmentCommand : public QUndoCommand
{
public:
    DeleteDepartmentCommand(int pos, int n, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    int position;
    int num;
    std::vector<Department> departmentData;
    CompanyDataModel *dataModel;
};

class DeleteEmployeeCommand : public QUndoCommand
{
public:
    DeleteEmployeeCommand(DataItem *parent, int pos, int n, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    DataItem *parentItem;
    int position;
    int num;
    std::vector<Employee> employeeData;
    CompanyDataModel *dataModel;
};

#endif // COMMANDS_H
