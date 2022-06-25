#include "charts.h"

// Потом Удалить
#include <QtCharts/QStackedBarSeries>


QChart *Charts::createBarChartNew(QVector<DataStorage> data, bool isColored)
{
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

    auto series = new QBarSeries();

    int i = 0;
    foreach (DataStorage elem, data)
    {
        QString legendHeader (elem.key);
        QBarSet *set = new QBarSet(legendHeader);
        *set << elem.value;
        if (!isColored)
        {
            auto color_ = i % 2 ? Qt::black : Qt::lightGray;
            set->setBrush(QBrush(color_, Qt::SolidPattern));
        }
        series->append(set);
        i++;
    }

    chart->removeAllSeries();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations); // Красивая анимация
    chart->createDefaultAxes();

    return chart;
}

QChart *Charts::createPieChartNew(QVector<DataStorage> data, bool isColored)
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

    auto series = new QPieSeries();

    int i = 0;
    foreach (DataStorage elem, data)
    {
        QString legendHeader (elem.key);
        series->append(legendHeader, elem.value);
        if (!isColored)
        {
            auto color_ = i % 2 ? Qt::black : Qt::lightGray;
            series->slices().at(i)->setBrush(QBrush(color_, Qt::SolidPattern));
        }
        i++;
    }

    chart->removeAllSeries();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations); // Красивая анимация
    chart->createDefaultAxes();

    return chart;
}

// Временно для 1ого тестового примера, пока не разберусь как запускать без него

// Рандомная генерация
DataTable Charts::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    // set seed for random stuff
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // generate random data
    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) 10 / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}

// Создание тест примера BarChart
QChart *Charts::createBarChart(int valueCount) const
{

    auto m_dataTable = generateRandomData(3, 10, 7);

    Q_UNUSED(valueCount);
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

    QStackedBarSeries *series = new QStackedBarSeries(chart);

    for (int i(0); i < m_dataTable.count(); i++)
    {
        QBarSet *set = new QBarSet("Bar set " + QString::number(i));

        for (const Data &data : m_dataTable[i])
        {
            *set << data.first.y();
        }
        series->append(set);
    }

    chart->removeAllSeries();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::SeriesAnimations); // Красивая анимация
    chart->createDefaultAxes();


    return chart;
}

void Charts::reDrawChart() const
{
    //chart_drawChart(data_);
}

void Charts::drawChart(const DataStorage& data)
{
 //   data_ = data;
}
