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


//Для SQLITE
class ChartDataSqlite
{
public:
    QVector <DataStorage> getData (QString path_);
};

#endif // DATA_H
