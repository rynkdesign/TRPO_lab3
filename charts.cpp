#include "charts.h"

// Отрисовка Вертикальных Диаграмм
void barChartDrawing::drawChart(QVector<DataStorage> data, bool isColored, QChart* chart_)
{
    chart_->setTitle("Bar chart"); // Заголовок над Диаграммой

    QBarSeries *series = new QBarSeries{chart_}; // Задаём серии данных для Вертикальной диаграммы

    int i = 0; // Переменная для подсчёта кол-ва считываемых данных из базы
    foreach (DataStorage elem, data) // Идём по каждому элементу
    {
        QString legendHeader (elem.key); // Записываем Ключ
        QBarSet *set = new QBarSet(legendHeader); // Задаём набор данных для 1 вертикальной диаграммы
        if (!isColored) // Отрисовка цвета
        {
            auto color_ = i % 2 ? Qt::black : Qt::lightGray;
            set->setBrush(QBrush(color_, Qt::SolidPattern));
        }
        *set << elem.value; // Добавляем в набор значение
        series->append(set); // Добавлеяем набор в серию
        i++;
    }

    chart_->removeAllSeries(); // Очищаем предыдущие серии
    chart_->addSeries(series); // Добавляем серию в Диаграмму
    chart_->setAnimationOptions(QChart::SeriesAnimations); // Красивая анимация
    chart_->createDefaultAxes(); // Устанавливаем стандартные оси
}

// Отрисовка Круговых Диаграмм
void pieChartDrawing::drawChart(QVector<DataStorage> data, bool isColored, QChart* chart_)
{
    chart_->setTitle("Pie chart"); // Заголовок над Диаграммой

    QPieSeries *series = new QPieSeries{chart_}; // Задаём серии данных для Круговой диаграммы

    int i = 0; // Переменная для подсчёта кол-ва считываемых данных из базы
    foreach (DataStorage elem, data) // Идём по каждому элементу
    {
        QString legendHeader (elem.key); // Записываем Ключ
        series->append(legendHeader, elem.value); // Добавлеяем ключ и значение в серию
        if (!isColored) // Отрисовка цвета
        {
            auto color_ = i % 2 ? Qt::black : Qt::lightGray;
            series->slices().at(i)->setBrush(QBrush(color_, Qt::SolidPattern));
        }
        i++;
    }

    chart_->removeAllSeries(); // Очищаем предыдущие серии
    chart_->addSeries(series); // Добавляем серию в Диаграмму
    chart_->setAnimationOptions(QChart::SeriesAnimations); // Красивая анимация
    chart_->createDefaultAxes(); // Устанавливаем стандартные оси
}

// Ф-ия перерисовки Диаграммы
void Charts::reDrawChart() const
{
    IOCContainer::instance().GetObject<IChartDrawing>()->drawChart(data_,isColored_,chart_);
}

// Ф-ия обновления данных
void Charts::updateData(const QString& filePath)
{
    // Перезаписываем дату
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

// Геттер для получения QT класса QChart
QChart* Charts::getChart()
{
    return chart_;
}

// Ф-ия смены цвета Диаграммы
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
