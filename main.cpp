/*
 * (c) Anna Chertova 2020
 * Main app logic
 */

#include "companydataloader.h"
#include "companydatasaver.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CompanyDataModel companyDataModel;
    MainWindow w(&companyDataModel);

    // Load/save xml utilities
    CompanyDataLoader dataLoader(&companyDataModel);
    CompanyDataSaver dataSaver(&companyDataModel);

    QObject::connect(&w, &MainWindow::loadCompanyData,
                     &dataLoader, &CompanyDataLoader::parseFile);
    QObject::connect(&w, &MainWindow::saveCompanyData,
                     &dataSaver, &CompanyDataSaver::saveFile);    

    QObject::connect(&dataLoader, &CompanyDataLoader::error,
                     &w, &MainWindow::errorDialog);
    QObject::connect(&dataLoader, &CompanyDataLoader::updateData,
                     &w, &MainWindow::updateView);
    QObject::connect(&dataSaver, &CompanyDataSaver::error,
                     &w, &MainWindow::errorDialog);

    QObject::connect(&w, &MainWindow::clearCompanyData,
                     &companyDataModel, &CompanyDataModel::clear);

    w.show();
    return a.exec();
}
