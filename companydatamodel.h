/*
 * (c) Anna Chertova 2020
 * Hierarchical model implementation for company data
 */

#ifndef COMPANYDATAMODEL_H
#define COMPANYDATAMODEL_H

#include <QAbstractItemModel>
#include <QUndoStack>

#include "department.h"
#include "dataitem.h"

class CompanyDataModel : public QAbstractItemModel
{

    Q_OBJECT

    friend class ChangeDataCommand;
    friend class AddDepartmentCommand;
    friend class AddEmployeeCommand;
    friend class DeleteDepartmentCommand;
    friend class DeleteEmployeeCommand;

public:

    explicit CompanyDataModel(QObject *parent = nullptr);
    ~CompanyDataModel();

    // QAbstactItemModel interface implementation

    virtual QModelIndex index(int row,
                              int column,
                              const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value,
                         int role) override;

    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int position,
                    int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position,
                    int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    // Helper functions for save/load

    void addDepartment(Department department);
    int getNumDepartments() const;
    Department getDepartment(int n) const;

    // Undo stack accessor (for GUI)
    QUndoStack *getUndoStack() const;

public slots:

    // clear data e.g. when the file is closed
    // to get ready to load new data from another file
    void clear();

    // to be able to change state when data is saved
    void onDataSaved();

private:

    // creates empty department item and fills it with default data
    DataItem *createEmptyDepartmentItem();
    // fills specified department item with data from department object
    void fillDepartmentItem(DataItem *departmentItem, Department department);
    // creates department object from specified item (filled with data)
    Department createDepartment(DataItem *departmentItem) const;    

    // fills specified employee item with data from employee object
    void fillEmployeeItem(DataItem* employeeItem, Employee employee);
    //creates employee object from specified item (filled with data)
    Employee createEmployee(DataItem *employeeItem) const;    

    // re-calculates department data (employee number & average salary)
    // needed when employee is added/removed/changed
    void updateDepartmentData(DataItem *departmentItem);

    // get department item row number
    int getRowNumber(DataItem *departmentItem) const;

private:

    enum Columns
    {
        RamificationColumn,
        DepartmentName = RamificationColumn,
        DepartmentNumEmployees,
        EmployeeSurname,
        EmployeeName,
        EmployeeMiddleName,
        EmployeePosition,
        EmployeeSalary,
        ColumnCount
    };

    std::vector<DataItem*> departmentItems;

    QUndoStack *undoStack;

};

#endif // COMPANYDATAMODEL_H
