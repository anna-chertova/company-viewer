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
    , standardModel(new QStandardItemModel(this))
{
    ui->setupUi(this);

    createActions();

    setWindowTitle("Company viewer - Unigine");

    // Adjust standard model
    standardModel->setHorizontalHeaderLabels({"Department name",
                                              "Num employees",
                                              "Avg salary"});

    QList<QStandardItem *> preparedRow = prepareRow("Network department",
                                                    "1",
                                                    "35000");
    QStandardItem *root = standardModel->invisibleRootItem();
    root->appendRow(preparedRow);

    QList<QStandardItem *> secondRow = prepareRow("Anna Chertova",
                                                  "Senior Sotware Developer",
                                                  "35000");
    preparedRow.first()->appendRow(secondRow);

    // Adjust treeView
    setCentralWidget(treeView);
    treeView->setModel(standardModel);
    treeView->expandAll();
    for(int i = 0; i < standardModel->rowCount(); ++i) {
        treeView->resizeColumnToContents(i);
    }
}

MainWindow::~MainWindow()
{
    // parent will destroy its children
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,
                             tr("Error opening file"),
                             tr("Could not open file"),
                             QMessageBox::Ok);
        return false;
    }

    emit loadCompanyData(&file);
    file.close();
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
    // Create menu actions
    QMenu *menuFile = menuBar()->addMenu(tr("&File"));
    QAction *actionOpen = menuFile->addAction(tr("&Open..."),
                                              this,
                                              &MainWindow::open);
    actionOpen->setShortcut(QKeySequence::Open);

    QAction *actionSaveAs = menuFile->addAction(tr("&Save as..."),
                                                this,
                                                &MainWindow::saveAs);
    actionSaveAs->setEnabled(false);

    QAction *actionClose = menuFile->addAction(tr("&Close"),
                                               this,
                                               &MainWindow::close);
    actionClose->setEnabled(false);
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
    QFileDialog dialog(this, tr("Open File"));
    initializeXmlFileDialog(dialog, QFileDialog::AcceptOpen);
    while (dialog.exec() == QDialog::Accepted &&
           !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeXmlFileDialog(dialog, QFileDialog::AcceptSave);
    while (dialog.exec() == QDialog::Accepted &&
           !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::close()
{
    /// TODO: clear ui connected with file contents here
}
