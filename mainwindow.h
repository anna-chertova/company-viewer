/*
 * (c) Anna Chertova 2020
 * This class manages main GUI
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class CompanyDataModel;
class QFile;
class QTreeView;
class QUndoStack;
class QUndoView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(CompanyDataModel *model, QWidget *parent = nullptr);
    ~MainWindow();

protected:

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

signals:

    void loadCompanyData(const QString &fileName);
    void saveCompanyData(const QString &fileName);
    void clearCompanyData();

public slots:

    void updateView();
    void errorDialog(const QString &problem, const QString &error);    

private slots:    

    void updateActions();

    void open();
    void saveAs();
    void close();

    void addDepartment();
    void removeDepartment();
    void addEmployee();
    void removeEmployee();

    void about();

private:

    void createActions();
    void createUndoView();
    void createMenus();

private:

    Ui::MainWindow *ui;
    QTreeView *treeView;
    QUndoView *undoView;
    CompanyDataModel *dataModel;

    QAction *actionOpen;
    QAction *actionSaveAs;
    QAction *actionClose;

    QMenu *menuEdit;

    QAction *actionAddDepartment;
    QAction *actionAddEmployee;
    QAction *actionRemoveDepartment;
    QAction *actionRemoveEmployee;

    QAction *actionUndo;
    QAction *actionRedo;

};
#endif // MAINWINDOW_H
