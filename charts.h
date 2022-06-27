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

class IChartDrawing
{
public:
    virtual void drawChart(QVector <DataStorage> data, bool isColored = false, QChart* chart_ = new QChart()) = 0;
};

class barChartDrawing : public IChartDrawing
{
public:
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_= new QChart());
};

class pieChartDrawing : public IChartDrawing
{
public:
    virtual void drawChart(QVector <DataStorage> data, bool isColored = true, QChart* chart_= new QChart());
};

class Charts
{
public:
    Charts(): chart_( new QChart()), isColored_ (true){}

    QChart* getChart();

    void updateData(const QString& filePath);
    void reDrawChart() const;
    void changeColor();

private:

    QChart *chart_;
    QVector <DataStorage> data_;
    bool isColored_;
};

#endif // CHARTS_H
