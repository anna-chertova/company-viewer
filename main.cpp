#include "mainwindow.h"
#include "companydata.h"
#include "companydataloader.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    CompanyData companyData;
    CompanyDataLoader dataLoader;

    QObject::connect(&dataLoader, &CompanyDataLoader::newDepartment,
                     &companyData, &CompanyData::addDepartment);
    QObject::connect(&dataLoader, &CompanyDataLoader::newDepartment,
                     &w, &MainWindow::update);
    QObject::connect(&w, &MainWindow::loadCompanyData,
                     &dataLoader, &CompanyDataLoader::parseFile);    
    QObject::connect(&dataLoader, &CompanyDataLoader::error,
                     &w, &MainWindow::errorDialog);
    QObject::connect(&w, &MainWindow::clearCompanyData,
                     &companyData, &CompanyData::clear);

    w.setModel(companyData.getModel());

    w.show();
    return a.exec();
}
