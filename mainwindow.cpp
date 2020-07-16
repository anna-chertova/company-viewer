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
    treeView->expandAll();
    int row_count = model->rowCount();
    for(int i = 0; i < row_count; ++i) {
        treeView->resizeColumnToContents(i);
    }
}

bool MainWindow::loadFile(const QString &fileName)
{
    /// TODO: move actual file loading into CompanyDataLoader logic
    /// but how we return file opening error from that???
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

