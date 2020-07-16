/*
 * (c) Anna Chertova 2020
 * This class manages main GUI
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QTreeView;
class QAbstractItemModel;
class QFile;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setModel(QAbstractItemModel *model);
    bool loadFile(const QString &fileName);

signals:
    void loadCompanyData(QFile *file);

private slots:
    void open();
    void saveAs();
    void close();

private:
    void createActions();

private:
    Ui::MainWindow *ui;
    QTreeView *treeView;
    QMenu *menuFile;
    QAction *actionOpen;
    QAction *actionSaveAs;
    QAction *actionClose;

};
#endif // MAINWINDOW_H
