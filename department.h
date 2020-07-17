/*
 * (c) Anna Chertova 2020
 * Specific data types used for this app
 */

#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <cmath>
#include <vector>
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
    std::vector<Employee> employees;

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
};

#endif // DEPARTMENT_H
