#ifndef CHARTS_H
#define CHARTS_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include <QJsonDocument>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts>
#include "data.h"
#include "ioccontainer.h"

// Интерфейс IOC контейнера для отрисовки Диаграммы
// У нас есть ф-ия drawChart, которая отрисовывает Диаграммы
// И мы можем отдельно написать drawChart для какого-нибудь нового типа Диаграмм, не изменяя IChartDrawing
class IChartDrawing
{
public:
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_ = new QChart()) = 0;
};

// Отрисовка Вертикальных Диаграмм
class barChartDrawing : public IChartDrawing
{
public:
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_= new QChart()); // Параметры по умолчанию: Цветная
};

// Отрисовка Круговых Диаграмм
class pieChartDrawing : public IChartDrawing
{
public:
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_= new QChart()); // Параметры по умолчанию: Цветная
};

// Класс для работы с Диаграммами
class Charts
{
public:
    Charts(): chart_( new QChart()), isColored_ (true){} // Задаём параметры по умолчанию

    QChart* getChart(); // Геттер для получения QT класса QChart

    void updateData(const QString& filePath); // Ф-ия обновления данных
    void reDrawChart() const; // Ф-ия перерисовки Диаграммы
    void changeColor(); // Ф-ия смены цвета

private:

    QChart *chart_; // Для работы с диаграммами
    QVector <DataStorage> data_; // Переменная хранящая key (ключ данных, дата-время) и value (значение данных) из наших таблиц
    bool isColored_; // Переменная определяющая цвет Диаграммы
};

#endif // CHARTS_H
