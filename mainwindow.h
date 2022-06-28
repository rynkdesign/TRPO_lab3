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
#include "data.h"
#include "charts.h"

class MainWindow : public QWidget
{
    Q_OBJECT
private slots:

    void slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected); // Слот обработки выбора конкретного файла
    void slotChooseDirectory(); // Слот обработки выбора Папки для отображения файлов
    void slotSelectionComboboxChanged(); // Слот обработки выбора Типа Диаграмм с помощью Combobox (Выпадающего списка)
    void slotSelectionColorChanged(); // Слот обработки выбора цвета: Цветной/ЧБ с помощью Checkbox (Выбор: Да/Нет)
    void slotSaveToPdf(); // Слот обработки сохранения Диаграммы в PDF

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QFileSystemModel *fileModel; // Модель файловой системы
    QTableView *tableView; // Модель табличного представления
    QString homePath; // Домашний путь директории
    QComboBox* boxType; // Выпадающий список с типами Диаграмм
    QCheckBox *checkboxColor; // Чекбокс для смены цвета Диаграммы

    // Анонимная структура для взаимодействия с Диаграммой
    struct
    {
        Charts* chart;
        QChartView* chartView;
    } chartSettings;
};

#endif // MAINWINDOW_H
