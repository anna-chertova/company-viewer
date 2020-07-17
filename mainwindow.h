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

signals:
    void loadCompanyData(const QString &fileName);
    void saveCompanyData(const QString &fileName);
    void clearCompanyData();

public slots:
    void update();
    void errorDialog(const QString &problem, const QString &error);

private slots:
    void open();
    void saveAs();
    void close();

private:
    void createActions();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);

private:
    Ui::MainWindow *ui;
    QTreeView *treeView;
    QMenu *menuFile;
    QAction *actionOpen;
    QAction *actionSaveAs;
    QAction *actionClose;

};
#endif // MAINWINDOW_H
