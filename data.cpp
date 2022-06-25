#include "data.h"
#include <QMessageBox>
#include <QtSql>

QVector <DataStorage> ChartDataSqlite::getData (QString path_)
{
    // Вектор для наших данных. Формат: Ключ(дата-время), значение
    QVector <DataStorage> data;

    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName(path_);

    // Проверяем на открытие
    if (!dbase.open())
    {
        QMessageBox messageBox;
        messageBox.setText("Невозможно открыть базу" + path_);
        messageBox.exec();
    }

    // Собираем данные в data
    else
    {
        QSqlQuery query("SELECT * FROM " + dbase.tables().takeFirst());
        int i = 0;
        while (query.next() && i < 10) // Т.к. данные огромные + требования к интерфейсу, то берем только первые 10 строк
        {
            i++;
            QString key = query.value(0).toString(); // Берем ключ превращаем в строку
            double value = query.value(1).toDouble(); // Берем значение превращаем в double
            data.push_back(DataStorage(key, value));
        }
    }

    return data;
}
