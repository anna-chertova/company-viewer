#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
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

static void initializeXmlFileDialog(QFileDialog &dialog,
                                    QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if(firstDialog) {
        firstDialog = false;
        const QStringList filesLocations = QStandardPaths::standardLocations(
                    QStandardPaths::DocumentsLocation);
        dialog.setDirectory(filesLocations.isEmpty() ?
                                QDir::currentPath() : filesLocations.last());
    }

    QStringList mimeTypeFilters({"application/xml"});
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("application/xml");
    if(acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("xml");
}

void MainWindow::open()
{
    close();
    QFileDialog dialog(this, tr("Open File"));
    initializeXmlFileDialog(dialog, QFileDialog::AcceptOpen);
    while (dialog.exec() == QDialog::Accepted &&
           !loadFile(dialog.selectedFiles().first())) {}
    actionSaveAs->setEnabled(true);
    actionClose->setEnabled(true);
}

void MainWindow::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeXmlFileDialog(dialog, QFileDialog::AcceptSave);
    while (dialog.exec() == QDialog::Accepted &&
           !saveFile(dialog.selectedFiles().first())) {}
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

bool MainWindow::loadFile(const QString &fileName)
{
    /// TODO: think about behavior when error occurred while openingin file
    emit loadCompanyData(fileName);
    setWindowTitle("Company Viewer - " + fileName);
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    emit saveCompanyData(fileName);
    return true;
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
