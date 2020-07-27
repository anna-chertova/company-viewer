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
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &MainWindow::updateActions);
    updateView();
    updateActions();
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
    actionAddDepartment->setEnabled(true);
    actionRemoveDepartment->setEnabled(true);
    actionAddEmployee->setEnabled(true);
    actionRemoveEmployee->setEnabled(true);
    menuEdit->setEnabled(true);
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
    actionAddDepartment->setEnabled(false);
    actionRemoveDepartment->setEnabled(false);
    actionAddEmployee->setEnabled(false);
    actionRemoveEmployee->setEnabled(false);
    menuEdit->setEnabled(false);
    emit clearCompanyData();
}

void MainWindow::updateView()
{
    treeView->expandAll();
    int row_count = treeView->model()->rowCount();
    for(int i = 0; i < row_count; ++i) {
        treeView->resizeColumnToContents(i);
    }
}

void MainWindow::updateActions()
{
    const bool hasCurrent = treeView->selectionModel()->currentIndex().isValid();

    if (hasCurrent) {
        treeView->closePersistentEditor(treeView->selectionModel()->currentIndex());

        const int row = treeView->selectionModel()->currentIndex().row();
        const int column = treeView->selectionModel()->currentIndex().column();
        if (treeView->selectionModel()->currentIndex().parent().isValid()) {
            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
            actionRemoveEmployee->setEnabled(true);
            actionRemoveDepartment->setEnabled(false);
        }
        else {
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
            actionRemoveDepartment->setEnabled(true);
            actionRemoveEmployee->setEnabled(false);
        }
    }
}

void MainWindow::addDepartment()
{

}

void MainWindow::removeDepartment()
{

}

void MainWindow::addEmployee()
{

}

void MainWindow::removeEmployee()
{

}

void MainWindow::undo()
{

}

void MainWindow::redo()
{

}

void MainWindow::errorDialog(const QString &problem, const QString &error)
{
    QMessageBox::warning(this, problem, error, QMessageBox::Ok);
}

void MainWindow::createActions()
{
    // Create menu actions

    // File menu
    QMenu *menuFile = menuBar()->addMenu(tr("&File"));
    actionOpen = menuFile->addAction(tr("&Open..."),
                                     this,
                                     &MainWindow::open);
    actionOpen->setShortcut(QKeySequence::Open);

    actionSaveAs = menuFile->addAction(tr("&Save as..."),
                                       this,
                                       &MainWindow::saveAs);
    actionSaveAs->setEnabled(false);

    actionClose = menuFile->addAction(tr("&Close"), this, &MainWindow::close);
    actionClose->setEnabled(false);

    // Edit menu
    menuEdit = menuBar()->addMenu(tr("&Edit"));
    menuEdit->setEnabled(false);

    actionAddDepartment = menuEdit->addAction(tr("Add department"), this, &MainWindow::addDepartment);
    actionAddDepartment->setEnabled(true);
    actionRemoveDepartment = menuEdit->addAction(tr("Remove department"), this, &MainWindow::removeDepartment);
    actionRemoveDepartment->setEnabled(false);
    actionAddEmployee = menuEdit->addAction(tr("Add employee"), this, &MainWindow::addEmployee);
    actionAddEmployee->setEnabled(true);
    actionRemoveEmployee = menuEdit->addAction(tr("Remove employee"), this, &MainWindow::removeEmployee);
    actionRemoveEmployee->setEnabled(false);

    actionUndo = menuEdit->addAction(tr("&Undo"), this, &MainWindow::undo);
    actionUndo->setShortcut(QKeySequence::Undo);
    actionUndo->setEnabled(false);

    actionRedo = menuEdit->addAction(tr("&Redo"), this, &MainWindow::redo);
    actionRedo->setShortcut(QKeySequence::Redo);
    actionRedo->setEnabled(false);

    connect(menuEdit, &QMenu::aboutToShow, this, &MainWindow::updateActions);
}
