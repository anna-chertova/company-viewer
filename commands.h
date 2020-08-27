/*
 * (c) Anna Chertova 2020
 * Command objects stored in undo/redo stack
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

    // Command to change data
    // dRow - department row number
    // eRow - employee row number (if employee is changed, otherwise use -1)
    // col - column number to be changed
    // oldVal - old value
    // newVal - new value
    // model - pointer to data model object
    ChangeDataCommand(int dRow, int eRow, int col, const QVariant &oldVal,
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

    // Command to add department
    // pos - where to add
    // n - number of departments to be added
    // model - pointer to a model object
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

    // Command to add employee
    // dRow - parent department row number
    // pos - where to add
    // n - number of employees to be added
    // model - pointer to a model object
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

    // Command to delete department
    // pos - where to delete
    // n - number of departments to be deleted
    // model - pointer to a model object
    DeleteDepartmentCommand(int pos, int n, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    int position;
    int num;
    // saves department data being deleted
    // (to be able to restore during redo)
    std::vector<Department> departmentData;
    CompanyDataModel *dataModel;
};


class DeleteEmployeeCommand : public QUndoCommand
{

public:

    // Command to delete employee
    // dRow - parent department row number
    // pos - where to delete
    // n - number of employees to be deleted
    // model - pointer to a model object
    DeleteEmployeeCommand(int dRow, int pos, int n, CompanyDataModel *model);

    void undo() override;
    void redo() override;

private:

    int departmentRow;
    int position;
    int num;
    // saves employee data being deleted
    // (to be able to restore during redo)
    std::vector<Employee> employeeData;
    CompanyDataModel *dataModel;
};

#endif // COMMANDS_H
