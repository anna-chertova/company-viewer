/*
 * (c) Anna Chertova 2020
 * This class manages company data as a separate entity
 */

#ifndef COMPANYDATA_H
#define COMPANYDATA_H

#include <cmath>
#include <QList>
#include <QObject>
#include <QString>

class CompanyData: public QObject
{
    Q_OBJECT

public:
    explicit CompanyData(QObject *parent = nullptr);

private:
    struct Employee {

        Employee(const QString& n, const QString &p, int s):
            name(n), position(p), salary(s) {
        }

        QString name;
        QString position;
        int salary;
    };

    struct Department {
        QString name;
        QList<Employee> employees;

        Department(const QString& n): name(n) {};

        int getNumEmployees() const {
            return employees.count();
        }

        int getAvgSalary() const {
            int sum = 0;
            for (auto e : employees) {
                sum += e.salary;
            }
            return std::round(static_cast<float>(sum)/
                              static_cast<float>(employees.count()));
        }

        void addEmployee(const QString& name,
                         const QString& position,
                         int salary) {
            employees.append(Employee(name, position, salary));
        }
    };

    QList<Department> departments;

    void addDepartment(const QString &name);

    private slots:
        void onNewDepartment(const QString &name);
};

#endif // COMPANYDATA_H
