#include "mainwindow.h"
#include "companydata.h"
#include "companydataloader.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;    
    w.show();

    CompanyData companyData;
    CompanyDataLoader dataLoader;

    QObject::connect(&dataLoader, &CompanyDataLoader::newDepartment, &companyData, &CompanyData::addDepartment);
    QObject::connect(&w, &MainWindow::loadCompanyData, &dataLoader, &CompanyDataLoader::parseFile);

    return a.exec();
}
