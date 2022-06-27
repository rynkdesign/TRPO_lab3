#include "charts.h"

void barChartDrawing::drawChart(QVector<DataStorage> data, bool isColored, QChart* chart_)
{
    chart_->setTitle("Bar chart");

    QBarSeries *series = new QBarSeries{chart_};

    int i = 0;
    foreach (DataStorage elem, data)
    {
        QString legendHeader (elem.key);
        QBarSet *set = new QBarSet(legendHeader);
        if (!isColored)
        {
            auto color_ = i % 2 ? Qt::black : Qt::lightGray;
            set->setBrush(QBrush(color_, Qt::SolidPattern));
        }
        *set << elem.value;
        series->append(set);
        i++;
    }

    chart_->removeAllSeries();
    chart_->addSeries(series);
    chart_->setAnimationOptions(QChart::SeriesAnimations); // Красивая анимация
    chart_->createDefaultAxes();
}

void pieChartDrawing::drawChart(QVector<DataStorage> data, bool isColored, QChart* chart_)
{
    chart_->setTitle("Pie chart");

    QPieSeries *series = new QPieSeries{chart_};

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

    chart_->removeAllSeries();
    chart_->addSeries(series);
    chart_->setAnimationOptions(QChart::SeriesAnimations); // Красивая анимация
    chart_->createDefaultAxes();
}

void Charts::reDrawChart() const
{
    IOCContainer::instance().GetObject<IChartDrawing>()->drawChart(data_,isColored_,chart_);
}

void Charts::updateData(const QString& filePath)
{
    data_ = IOCContainer::instance().GetObject<IChartData>()->getData(filePath);

    if (data_.isEmpty())
    {
        QMessageBox messageBox;
        messageBox.setText("Файл пустой");
        messageBox.exec();
        return;
    }
    reDrawChart();
}

QChart* Charts::getChart()
{
    return chart_;
}

void Charts::changeColor()
{
    if(isColored_ == true)
    {
        isColored_ = false;
    }
    else
    {
        isColored_ = true;
    }
}
