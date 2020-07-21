#include "companydataloader.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    CompanyDataModel companyDataModel;

    CompanyDataLoader dataLoader(&companyDataModel);
    //CompanyDataSaver dataSaver(&companyData);

    QObject::connect(&w, &MainWindow::loadCompanyData,
                     &dataLoader, &CompanyDataLoader::parseFile);
    /*QObject::connect(&w, &MainWindow::saveCompanyData,
                     &dataSaver, &CompanyDataSaver::saveFile);
    QObject::connect(&w, &MainWindow::clearCompanyData,
                     &companyData, &CompanyData::clear);*/

    QObject::connect(&dataLoader, &CompanyDataLoader::error,
                     &w, &MainWindow::errorDialog);
    QObject::connect(&dataLoader, &CompanyDataLoader::updateData,
                     &w, &MainWindow::update);
    /*QObject::connect(&dataSaver, &CompanyDataSaver::error,
                     &w, &MainWindow::errorDialog);
    */
    w.setModel(&companyDataModel);
    w.show();
    return a.exec();
}
