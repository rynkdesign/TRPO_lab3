#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QPushButton>
#include <QTableView>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include "themewidget.h"
#include "data.h"

class MainWindow : public QWidget
{
    Q_OBJECT
private slots:

    void slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void slotChooseDirectory();
    void slotSelectionComboboxChanged();
    void slotSelectionColorChanged();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QFileSystemModel *fileModel;
    QTableView *tableView;
    QString homePath;
    QComboBox* boxType;
    ThemeWidget *themeWidget;
    QChartView *chartView;
    QChart *chartBar;
    QCheckBox *checkboxColor;
};

#endif // MAINWINDOW_H
