#include "mainwindow.h"
#include "companydataloader.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;    
    w.show();

    CompanyDataLoader dataLoader;
    QObject::connect(&w, &MainWindow::loadCompanyData, &dataLoader, &CompanyDataLoader::parseFile);

    return a.exec();
}
