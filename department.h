/*
 * (c) Anna Chertova 2020
 * Specific data types used for this app
 */

#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <cmath>
#include <list>
#include <QString>

struct Employee {

    Employee() {};

    Employee(const QString& sn,
             const QString& n,
             const QString& mn,
             const QString &p,
             int s):
        surname(sn), name(n), middlename(mn), position(p), salary(s) {
    }

    QString surname;
    QString name;
    QString middlename;
    QString position;
    int salary;
};

struct Department {
    QString name;
    std::list<Employee> employees;

    Department() {};
    Department(const QString& n): name(n) {};

    int getNumEmployees() const {
        return employees.size();
    }

    int getAvgSalary() const {
        int sum = 0;
        for (auto e : employees) {
            sum += e.salary;
        }
        return std::round(static_cast<float>(sum)/
                          static_cast<float>(employees.size()));
    }

    void addEmployee(const QString &surname,
                     const QString& name,
                     const QString& middlename,
                     const QString& position,
                     int salary) {
        employees.push_back(Employee(surname,
                                     name,
                                     middlename,
                                     position,
                                     salary));
    }
};

#endif // DEPARTMENT_H
