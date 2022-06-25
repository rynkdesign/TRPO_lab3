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
#include "data.h"

// Потом удалить
typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Charts
{
public:
    Charts();
    QChart *createBarChartNew (QVector <DataStorage>, bool isColored); // НОВАЯ ФУНКЦИЯ ДЛЯ BARCHART
    QChart *createPieChartNew (QVector<DataStorage> data, bool isColored); // НОВАЯ ФУНКЦИЯ ДЛЯ PIECHART
    QChart *createBarChart(int valueCount) const; // Потом удалить

    void drawChart(const DataStorage& data);
    void reDrawChart() const;

    QChart *chart_; // ChartParametrs
    QVector <DataStorage> data_;// ChartParametrs

private:
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const; // Потом удалить



    QPieSeries *series;

    // Потом удалить
    DataTable m_dataTable;

};

#endif // CHARTS_H
