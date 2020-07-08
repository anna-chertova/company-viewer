#include <QFileDialog>
#include <QHeaderView>
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
    , standardModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    setWindowTitle("Company viewer - Unigine");
    createActions();

    setCentralWidget(treeView);

    standardModel->setHorizontalHeaderLabels({"Department name", "Num employees", "Avg salary"});

    QList<QStandardItem *> preparedRow = prepareRow("Network department", "1", "35000");
    QStandardItem *root = standardModel->invisibleRootItem();
    root->appendRow(preparedRow);

    QList<QStandardItem *> secondRow = prepareRow("Anna Chertova", "Senior Sotware Developer", "35000");
    preparedRow.first()->appendRow(secondRow);

    treeView->setModel(standardModel);
    treeView->expandAll();
    for(int i = 0; i < standardModel->rowCount(); ++i) {
        treeView->resizeColumnToContents(i);
    }
}

MainWindow::~MainWindow()
{
    //delete standardModel;
    //delete treeView;
    //delete ui;
}

bool MainWindow::loadFile(const QString &fileName)
{
    return true;
}

QList<QStandardItem *> MainWindow::prepareRow(const QString &first,
                                              const QString &second,
                                              const QString &third) const
{
    return {new QStandardItem(first),
            new QStandardItem(second),
            new QStandardItem(third)};
}

void MainWindow::createActions()
{
    QMenu *menuFile = menuBar()->addMenu(tr("&File"));
    QAction *actionOpen = menuFile->addAction(tr("&Open..."), this, &MainWindow::open);
    actionOpen->setShortcut(QKeySequence::Open);

    QAction *actionSaveAs = menuFile->addAction(tr("&Save as..."), this, &MainWindow::saveAs);
    actionSaveAs->setEnabled(false);

    QAction *actionClose = menuFile->addAction(tr("&Close"), this, &MainWindow::close);
    actionClose->setEnabled(false);
}

static void initializeXmlFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if(firstDialog) {
        firstDialog = false;
        const QStringList filesLocations = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
        dialog.setDirectory(filesLocations.isEmpty() ? QDir::currentPath() : filesLocations.last());
    }

    QStringList mimeTypeFilters({"application/xml"});
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("application/xml");
    if(acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("xml");
}

void MainWindow::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeXmlFileDialog(dialog, QFileDialog::AcceptOpen);
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeXmlFileDialog(dialog, QFileDialog::AcceptSave);
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::close()
{

}

