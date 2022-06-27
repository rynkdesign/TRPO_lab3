#include "data.h"
#include <QMessageBox>
#include <QtSql>

QVector <DataStorage> ChartDataSqlite::getData (QString path_)
{
    // Вектор для наших данных. Формат: Ключ(дата-время), значение
    QVector <DataStorage> data;

    static QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
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
            QString key = query.value(0).toString(); // Берем ключ превращаем в строку
            double value = query.value(1).toDouble(); // Берем значение превращаем в double
            data.push_back(DataStorage(key, value));
            i++;
        }
    }

    return data;
}

QVector <DataStorage> ChartDataJson::getData (QString path_)
{
    // Вектор для наших данных. Формат: Ключ(дата-время), значение
    QVector <DataStorage> data;

    QString fileInString;
    QFile file;
    file.setFileName(path_);

    // Проверяем на открытие
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox messageBox;
        messageBox.setText("Невозможно открыть базу" + path_);
        messageBox.exec();
    }

    fileInString = file.readAll();
    file.close();

    // конвертируем в JSON
    QJsonDocument doc = QJsonDocument::fromJson(fileInString.toUtf8());

    if (!doc.isArray())
    {
        QMessageBox messageBox;
        messageBox.setText("Введите Json как Массив" + path_);
        messageBox.exec();
    }

    QJsonArray jsonArr = doc.array();
    int i = 0;
    foreach (const QJsonValue & value, jsonArr)
    {
        if (value.isObject() && i < 10)
        {
            QJsonObject obj = value.toObject();
            QString key = obj["Time"].toString();
            double value = obj["Value"].toDouble();

            data.push_back(DataStorage(key, value));
            i++;
        }
    }
    return data;
}
