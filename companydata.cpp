#include "companydata.h"

CompanyData::CompanyData(QObject *)
{

}

void CompanyData::addDepartment(const Department &department) {
    departments.push_back(department);
}
