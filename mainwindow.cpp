/*
 * (c) Anna Chertova 2020
 * GUI logic
 */

#include <QContextMenuEvent>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTreeView>
#include <QUndoStack>
#include <QUndoView>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , treeView(new QTreeView(this))    
{
    ui->setupUi(this);

    undoStack = new QUndoStack(this);

    createActions();
    createUndoView();
    createMenus();
    setCentralWidget(treeView);
}

MainWindow::~MainWindow()
{

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
    const QModelIndex curIndex = treeView->currentIndex();
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

    QModelIndex parentIndex = index.parent();

    // if employee row is selected
    // then take its parent's parent as a parent for the new department
    if(index.parent().isValid()) {
        parentIndex = parentIndex.parent();
    }

    if (!model->insertRow(index.row() + 1, parentIndex))
        return;

    updateActions();

    const QModelIndex child = model->index(index.row() + 1, 0, parentIndex);
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
    if(index.parent().isValid()) {
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
    const QModelIndex index = treeView->selectionModel()->currentIndex();
    QAbstractItemModel *model = treeView->model();
    if (!model->removeRow(index.row(), index.parent()))
        return;

    updateActions();
    updateView();
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
    actionOpen = new QAction(tr("&Open..."), this);
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, &QAction::triggered, this,&MainWindow::open);

    actionSaveAs = new QAction(tr("&Save as..."), this);
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    actionSaveAs->setEnabled(false);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);

    actionClose = new QAction(tr("&Close"), this);
    actionClose->setShortcut(QKeySequence::Close);
    actionClose->setEnabled(false);
    connect(actionClose, &QAction::triggered, this, &MainWindow::close);

    // Edit menu (Add/remove department/employee)

    actionAddDepartment = new QAction(tr("Add department"), this);
    actionAddDepartment->setEnabled(false);
    connect(actionAddDepartment,
            &QAction::triggered,
            this,
            &MainWindow::addDepartment);

    actionRemoveDepartment = new QAction(tr("Remove department"), this);
    actionRemoveDepartment->setEnabled(false);
    connect(actionRemoveDepartment,
            &QAction::triggered,
            this,
            &MainWindow::removeDepartment);

    actionAddEmployee = new QAction(tr("Add employee"), this);
    actionAddEmployee->setEnabled(false);
    connect(actionAddEmployee,
            &QAction::triggered,
            this,
            &MainWindow::addEmployee);

    actionRemoveEmployee = new QAction(tr("Remove employee"), this);
    actionRemoveEmployee->setEnabled(false);
    connect(actionRemoveEmployee,
            &QAction::triggered,
            this,
            &MainWindow::removeEmployee);


    // Edit menu (Undo/redo)
    actionUndo = undoStack->createUndoAction(this, tr("&Undo"));
    actionUndo->setShortcut(QKeySequence::Undo);
    actionUndo->setEnabled(false);

    actionRedo = undoStack->createRedoAction(this, tr("&Redo"));
    actionRedo->setShortcut(QKeySequence::Redo);
    actionRedo->setEnabled(false);
}

void MainWindow::createUndoView()
{
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}

void MainWindow::createMenus()
{
    // File menu
    QMenu *menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionSaveAs);
    menuFile->addAction(actionClose);

    // Edit menu
    menuEdit = menuBar()->addMenu(tr("&Edit"));
    menuEdit->setEnabled(false);
    menuEdit->addAction(actionAddDepartment);
    menuEdit->addAction(actionRemoveDepartment);
    menuEdit->addAction(actionAddEmployee);
    menuEdit->addAction(actionRemoveEmployee);
    menuEdit->addSeparator();
    menuEdit->addAction(actionUndo);
    menuEdit->addAction(actionRedo);
    connect(menuEdit, &QMenu::aboutToShow, this, &MainWindow::updateActions);

    // Help menu
    QMenu *menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->addAction(tr("&About"), this, &MainWindow::about);
}
