#include <QContextMenuEvent>
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

#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex curIndex = treeView->indexAt(event->pos());
    if(!curIndex.isValid())
        return;

    QMenu menu(this);
    menu.addAction(actionAddDepartment);
    menu.addAction(actionRemoveDepartment);
    menu.addAction(actionAddEmployee);
    menu.addAction(actionRemoveEmployee);
    menu.exec(event->globalPos());

}
#endif // QT_NO_CONTEXTMENU

void MainWindow::open()
{
    close();

    const QStringList filesLocations = QStandardPaths::standardLocations(
                QStandardPaths::DocumentsLocation);
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                filesLocations.isEmpty() ?
                    QDir::currentPath() : filesLocations.last(),
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
    const bool hasCurrent =
            treeView->selectionModel()->currentIndex().isValid();

    if (hasCurrent) {
        treeView->closePersistentEditor(
                    treeView->selectionModel()->currentIndex());

        const int row = treeView->selectionModel()->currentIndex().row();
        const int column = treeView->selectionModel()->currentIndex().column();
        if (treeView->selectionModel()->currentIndex().parent().isValid()) {
            statusBar()->showMessage(
                        tr("Position: (%1,%2)").arg(row).arg(column));
            actionRemoveEmployee->setEnabled(true);
            actionRemoveDepartment->setEnabled(false);
        }
        else {
            statusBar()->showMessage(
                        tr("Position: (%1,%2) in top level").arg(row).arg(column));
            actionRemoveDepartment->setEnabled(true);
            actionRemoveEmployee->setEnabled(false);
        }
    }
}

void MainWindow::addDepartment()
{
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = treeView->model();

    if (!model->insertRow(index.row() + 1, index.parent()))
        return;

    updateActions();

    const QModelIndex child = model->index(index.row() + 1, 0, index.parent());
    model->setData(child, QVariant(tr("[Enter name]")), Qt::EditRole);

    updateView();
}

void MainWindow::removeDepartment()
{
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = treeView->model();
    if (!model->removeRow(index.row(), index.parent()))
        return;
    updateActions();
    updateView();
}

void MainWindow::addEmployee()
{
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = treeView->model();

    QModelIndex parentIndex = index;
    int childRow = 0;

    // if employee row is selected,
    // then take its parent as a parent for the new employee
    // otherwise currently selected department will be the parent
    if(index.parent().isValid()){
       parentIndex = index.parent();
       childRow = index.row() + 1;
    }

    if (!model->insertRow(childRow, parentIndex))
        return;

    updateActions();

    model->setData(
                model->index(childRow, 2, parentIndex),
                QVariant(tr("[Enter surname]")),
                Qt::EditRole);
    model->setData(
                model->index(childRow, 3, parentIndex),
                QVariant(tr("[Enter name]")),
                Qt::EditRole);
    model->setData(
                model->index(childRow, 4, parentIndex),
                QVariant(tr("[Enter middlename]")),
                Qt::EditRole);
    model->setData(
                model->index(childRow, 5, parentIndex),
                QVariant(tr("[Enter position]")),
                Qt::EditRole);
    model->setData(
                model->index(childRow, 6, parentIndex),
                QVariant(tr("[Enter salary]")),
                Qt::EditRole);

    updateView();
}

void MainWindow::removeEmployee()
{
    updateActions();
    updateView();
}

void MainWindow::undo()
{

}

void MainWindow::redo()
{

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Menu"),
                       tr("Company viewer - program for viewing and editing company files\n"
                          "(xml files containing list of company departments & employees)\n"
                          "\n(c) Anna Chertova 2020"));
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
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    actionSaveAs->setEnabled(false);

    actionClose = menuFile->addAction(tr("&Close"), this, &MainWindow::close);
    actionClose->setShortcut(QKeySequence::Close);
    actionClose->setEnabled(false);

    // Edit menu (Add/remove department/employee)
    menuEdit = menuBar()->addMenu(tr("&Edit"));
    menuEdit->setEnabled(false);

    actionAddDepartment = menuEdit->addAction(
                tr("Add department"),
                this,
                &MainWindow::addDepartment);
    actionAddDepartment->setEnabled(false);
    actionRemoveDepartment = menuEdit->addAction(
                tr("Remove department"),
                this,
                &MainWindow::removeDepartment);
    actionRemoveDepartment->setEnabled(false);
    actionAddEmployee = menuEdit->addAction(
                tr("Add employee"),
                this,
                &MainWindow::addEmployee);
    actionAddEmployee->setEnabled(false);
    actionRemoveEmployee = menuEdit->addAction(
                tr("Remove employee"),
                this,
                &MainWindow::removeEmployee);
    actionRemoveEmployee->setEnabled(false);

    // Edit menu (Undo/redo)
    menuEdit->addSeparator();
    actionUndo = menuEdit->addAction(tr("&Undo"), this, &MainWindow::undo);
    actionUndo->setShortcut(QKeySequence::Undo);
    actionUndo->setEnabled(false);

    actionRedo = menuEdit->addAction(tr("&Redo"), this, &MainWindow::redo);
    actionRedo->setShortcut(QKeySequence::Redo);
    actionRedo->setEnabled(false);

    // Help menu
    QMenu *menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->addAction(tr("&About"), this, &MainWindow::about);

    connect(menuEdit, &QMenu::aboutToShow, this, &MainWindow::updateActions);
}
