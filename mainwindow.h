#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QPushButton>
#include <QTreeView>
#include <QTableView>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include "themewidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT
private slots:

    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void slotChooseDirectory();


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QFileSystemModel *fileModel;
    QTableView *tableView;
    QString homePath;
};

#endif // MAINWINDOW_H
