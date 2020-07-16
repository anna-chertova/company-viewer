#include "companydata.h"

CompanyData::CompanyData(QObject *)
{
    standardModel.setHorizontalHeaderLabels({tr("Surname"),
                                             tr("Name"),
                                             tr("Middlename"),
                                             tr("Position"),
                                             tr("Salary")});

    /*
    QList<QStandardItem *> preparedRow = {"Network department",
                                          "1",
                                          "35000"};
    QStandardItem *root = standardModel->invisibleRootItem();
    root->appendRow(preparedRow);

    QList<QStandardItem *> secondRow = prepareRow("Anna Chertova",
                                                  "Senior Sotware Developer",
                                                  "35000");
    preparedRow.first()->appendRow(secondRow);*/
}
/*
QList<QStandardItem *> MainWindow::prepareRow(const QString &first,
                                              const QString &second,
                                              const QString &third) const
{
    return {new QStandardItem(first),
            new QStandardItem(second),
            new QStandardItem(third)};
}
*/

QAbstractItemModel *CompanyData::getModel()
{
    return &standardModel;
}

void CompanyData::addDepartment(const Department &department)
{
    departments.push_back(department);
}
