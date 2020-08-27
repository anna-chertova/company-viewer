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
    ChangeDataCommand(int dRow, int eRow, int col, const QVariant &olldVal,
                      const QVariant &newVal, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    int departmentRow;
    int employeeRow;
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
    CompanyDataModel *dataModel;
};

class AddEmployeeCommand : public QUndoCommand
{
public:
    AddEmployeeCommand(int dRow, int pos, int n, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    int departmentRow;
    int position;
    int num;
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
    DeleteEmployeeCommand(int dRow, int pos, int n, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    int departmentRow;
    int position;
    int num;
    std::vector<Employee> employeeData;
    CompanyDataModel *dataModel;
};

#endif // COMMANDS_H
