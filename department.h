/*
 * (c) Anna Chertova 2020
 * Specific data types used for this app
 */

#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <cmath>
#include <vector>
#include <QString>

struct Department;

struct Employee {

    /// TODO: remove pointer to department here
    Department *department;
    QString surname;
    QString name;
    QString middlename;
    QString position;
    int salary;

    Employee(Department *d): department(d) {};

    bool operator== (const Employee& other) const
    {
        return (surname == other.surname &&
                name == other.name &&
                middlename == other.middlename &&
                position == other.position);
    }
};

struct Department {
    QString name;
    std::vector<Employee> employees;

    Department() {};
    Department(const QString& n): name(n) {};

    int getNumEmployees() const {
        return employees.size();
    }

    int getAvgSalary() const {
        if (employees.empty())
            return 0;
        int sum = 0;
        for (auto e : employees) {
            sum += e.salary;
        }
        return std::round(static_cast<float>(sum)/
                          static_cast<float>(employees.size()));
    }

    bool operator== (const Department& other) const
    {
        return (name == other.name);
    }
};

#endif // DEPARTMENT_H
