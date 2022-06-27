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


class IChartData
{
public:
    virtual QVector <DataStorage> getData (QString path_) = 0;
};

//Для SQLITE
class ChartDataSqlite : public IChartData
{
public:
    QVector <DataStorage> getData (QString path_);
};

// Для JSON
class ChartDataJson : public IChartData
{
public:
    QVector <DataStorage> getData (QString path_);
};

#endif // DATA_H
