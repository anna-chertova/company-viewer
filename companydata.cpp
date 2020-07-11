#include "companydata.h"

CompanyData::CompanyData(QObject *)
{

}

void CompanyData::addDepartment(const QString &name) {
    departments.append(Department(name));
}

void CompanyData::onNewDepartment(const QString &name) {
    addDepartment(name);
}
