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

    QString surname;
    QString name;
    QString middlename;
    QString position;
    int salary;

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

    bool operator== (const Department& other) const
    {
        return (name == other.name);
    }
};

#endif // DEPARTMENT_H
