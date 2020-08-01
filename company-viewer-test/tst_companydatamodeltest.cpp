#include <QAbstractItemModelTester>
#include <QtTest>

#include "../companydatamodel.h"

// add necessary includes here

class CompanyDataModelTest : public QObject
{
    Q_OBJECT

public:
    CompanyDataModelTest();
    ~CompanyDataModelTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

CompanyDataModelTest::CompanyDataModelTest()
{

}

CompanyDataModelTest::~CompanyDataModelTest()
{

}

void CompanyDataModelTest::initTestCase()
{

}

void CompanyDataModelTest::cleanupTestCase()
{

}

void CompanyDataModelTest::test_case1()
{
    CompanyDataModel model;
    new QAbstractItemModelTester(&model);
}

QTEST_APPLESS_MAIN(CompanyDataModelTest)

#include "tst_companydatamodeltest.moc"
