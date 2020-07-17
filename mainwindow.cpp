#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTreeView>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , treeView(new QTreeView(this))    
{
    ui->setupUi(this);
    createActions();
    setCentralWidget(treeView);
}

MainWindow::~MainWindow()
{
    // parent will destroy its children
}

void MainWindow::setModel(QAbstractItemModel *model)
{
    treeView->setModel(model);
    update();
}

void MainWindow::open()
{
    close();

    const QStringList filesLocations = QStandardPaths::standardLocations(
                QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                filesLocations.isEmpty() ? QDir::currentPath() : filesLocations.last(),
                tr("XML files (*.xml)"));
    if(fileName.isNull()) return;
    /// TODO: think about behavior when error occurred while opening file
    emit loadCompanyData(fileName);
    setWindowTitle("Company Viewer - " + fileName);
    actionSaveAs->setEnabled(true);
    actionClose->setEnabled(true);
}

void MainWindow::saveAs()
{
    const QStringList filesLocations = QStandardPaths::standardLocations(
                QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save File As"),
                filesLocations.last(),
                tr("XML files (*.xml)"));
    if(fileName.isNull()) return;
    emit saveCompanyData(fileName);
    setWindowTitle("Company Viewer - " + fileName);
}

void MainWindow::close()
{
    setWindowTitle("Company Viewer");
    actionSaveAs->setEnabled(false);
    actionClose->setEnabled(false);
    emit clearCompanyData();
}

void MainWindow::update()
{
    treeView->expandAll();
    int row_count = treeView->model()->rowCount();
    for(int i = 0; i < row_count; ++i) {
        treeView->resizeColumnToContents(i);
    }
}

void MainWindow::errorDialog(const QString &problem, const QString &error)
{
    QMessageBox::warning(this, problem, error, QMessageBox::Ok);
}

void MainWindow::createActions()
{
    // Create menu actions
    menuFile = menuBar()->addMenu(tr("&File"));
    actionOpen = menuFile->addAction(tr("&Open..."),
                                              this,
                                              &MainWindow::open);
    actionOpen->setShortcut(QKeySequence::Open);

    actionSaveAs = menuFile->addAction(tr("&Save as..."),
                                                this,
                                                &MainWindow::saveAs);
    actionSaveAs->setEnabled(false);

    actionClose = menuFile->addAction(tr("&Close"),
                                               this,
                                               &MainWindow::close);
    actionClose->setEnabled(false);
}
