#ifndef DATA_H
#define DATA_H

#include <QString>


// Вектор для наших данных. Формат: Ключ(дата-время), значение
class DataStorage
{
public:
    QString key;
    double value;
    DataStorage (QString key_, double value_) { key = key_; value = value_; };
};

// Интерфейс IOC контейнера для чтения данных для отрисовки Диаграмм
// У нас есть ф-ия getData, которая возвращает данные из таблиц в формате QVector <DataStorage>
// И мы можем отдельно написать getData для какого-нибудь нового типа баз данных, не изменяя IChartData
class IChartData
{
public:
    virtual QVector <DataStorage> getData (QString path_) = 0;
};

// Чтение данных формата .sqlite
class ChartDataSqlite : public IChartData
{
public:
    QVector <DataStorage> getData (QString path_);
};

// Чтение данных формата .json
class ChartDataJson : public IChartData
{
public:
    QVector <DataStorage> getData (QString path_);
};

#endif // DATA_H
