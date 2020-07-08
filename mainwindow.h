#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QTreeView;
class QStandardItemModel;
class QStandardItem;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool loadFile(const QString &fileName);

private slots:
    void open();
    void saveAs();
    void close();

private:
    void createActions();

private:
    Ui::MainWindow *ui;

    QTreeView *treeView;
    QStandardItemModel *standardModel;
    QList<QStandardItem *> prepareRow(const QString &first,
                                      const QString &second,
                                      const QString &third) const;
};
#endif // MAINWINDOW_H
