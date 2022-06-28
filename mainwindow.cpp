#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QDebug>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QFileDialog>

// Определение s_typeId IOC Контейнера
int IOCContainer::s_typeId = 121;

namespace
{

// Можно ли печатать Диаграмму в PDF?
bool isCharCanPrintPDF = false;

}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
      //QMainWindow(parent)
{
    // Текущий путь
    QString homePath = QDir::homePath();

    // Устанавливаем размер главного окна
    setGeometry(100, 100, 1500, 500);
    setWindowTitle("PrintChart");

    // Определим файловой системы:
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(homePath);

    // Определим поиск файлов
    tableView = new QTableView;
    tableView->setModel(fileModel);

    // Добавление диаграммы
    chartSettings.chart = new Charts{};
    chartSettings.chartView = new QChartView{};

    // Ставим по умолчанию barChart
    IOCContainer::instance().RegisterInstance<IChartDrawing, barChartDrawing>();

    // Объявляем Buttons
    auto directoryButton = new QPushButton ("Открыть папку");
    auto printChartButton = new QPushButton ("Печать графика");
    checkboxColor = new QCheckBox("Черно-белый график");
    auto boxLabel = new QLabel("Выберите тип диаграммы");
    auto directoryLabel = new QLabel("Выберите файл БД:");

    boxType = new QComboBox(); // Выбор типа графика
    boxType->insertItem(1, QString("BarChart"));
    boxType->insertItem(2, QString("PieChart"));

    // Объявляем Layouts
    auto horizontalLayout=new QHBoxLayout(this);
    auto verticalLeftLayout = new QVBoxLayout();
    auto verticalRightLayout = new QVBoxLayout();
    auto chartLayout = new QHBoxLayout(); // Layout для кнопок над графиком

    horizontalLayout->addLayout(verticalLeftLayout);
    horizontalLayout->addLayout(verticalRightLayout);
    verticalRightLayout->addLayout(chartLayout);
    verticalLeftLayout->addWidget(tableView);
    verticalRightLayout->addWidget(chartSettings.chartView);
    verticalLeftLayout->addWidget(directoryLabel);
    verticalLeftLayout->addWidget(directoryButton); // Кнопка "Открыть папку"


    // Buttons над графиком
    chartLayout->addWidget(boxLabel); // Label над графиком
    chartLayout->addWidget(boxType); // Тип графика
    chartLayout->addWidget(checkboxColor); // Ч-б
    chartLayout->addWidget(printChartButton); // Печать в PDF

    // Для отслеживания выбранных элементов
    QItemSelectionModel *selectionModel = tableView->selectionModel();

    // Slots
    connect(directoryButton,&QPushButton::clicked,this,&MainWindow::slotChooseDirectory); // Кнопка выбора папки
    connect(boxType,SIGNAL(currentTextChanged(const QString&)),this,SLOT(slotSelectionComboboxChanged())); // Выбор типа графика
    connect(checkboxColor, SIGNAL(toggled(bool)), this, SLOT(slotSelectionColorChanged())); // Выбор Ч-Б

    // Выбор конкретного файла
    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &)));

    connect(printChartButton,SIGNAL(clicked()), this, SLOT(slotSaveToPdf())); // Печать в PDF

}

// Слот обработки выбора конкретного файла
void MainWindow::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexs =  selected.indexes(); // Взяли индекс у конкретного выбранного файла
    if (indexs.count() < 1)
    {
        return;
    }

    QString filePath{""};
    filePath = fileModel->filePath(indexs.constFirst()); // Получили полный путь к этому файлу

    if (filePath.endsWith(".sqlite")) // Если полный путь файла оканчивается на .sqlite
    {
        IOCContainer::instance().RegisterInstance<IChartData, ChartDataSqlite>(); // Задаём интерфейсу чтение SQLITE файлов
        chartSettings.chart->updateData(filePath); // Обновляем данные
        chartSettings.chartView->setChart(chartSettings.chart->getChart()); // Устанавливаем Диаграмму
        isCharCanPrintPDF = true; // Теперь её можно печатать
    }
    else if (filePath.endsWith(".json")) // Если полный путь файла оканчивается на .json
    {
        IOCContainer::instance().RegisterInstance<IChartData, ChartDataJson>();// Задаём интерфейсу чтение Json файлов
        chartSettings.chart->updateData(filePath); // Обновляем данные
        chartSettings.chartView->setChart(chartSettings.chart->getChart());// Устанавливаем Диаграмму
        isCharCanPrintPDF = true;// Теперь её можно печатать
    }
    else
    {
        isCharCanPrintPDF = false; // Нельзя печатать
        QMessageBox messageBox;
        messageBox.setText("Выберит файл формата .sqlite или .json");
        messageBox.exec();
    }
}

// Слот обработки выбора Папки для отображения файлов
void MainWindow::slotChooseDirectory()
{
    QFileDialog dialog{this};
    dialog.setFileMode(QFileDialog::Directory); // Открываем диалог
    if ( dialog.exec() )
    {
        homePath = dialog.selectedFiles().first(); // Получаем выбранную директорию из диалога
    }
    tableView->setRootIndex(fileModel->setRootPath(homePath)); // Устанавливаем в отображение файлов этот путь
}

// Слот обработки выбора Типа Диаграмм с помощью Combobox (Выпадающего списка)
void MainWindow::slotSelectionComboboxChanged()
{
    QString chartType = boxType->currentText(); // Получаем текст из Combobox

    if(chartType == "PieChart")
    {
        IOCContainer::instance().RegisterInstance<IChartDrawing, pieChartDrawing>(); // Задаём интерфейсу Круговую Диаграмму
        chartSettings.chart->reDrawChart(); // Перерисовываем
        return;
    }
    else if (chartType == "BarChart")
    {
        IOCContainer::instance().RegisterInstance<IChartDrawing, barChartDrawing>(); // Задаём интерфейсу Вертикальную Диаграмму
        chartSettings.chart->reDrawChart(); // Перерисовываем
        return;
    }
}

// Слот обработки выбора цвета: Цветной/ЧБ с помощью Checkbox (Выбор: Да/Нет)
void MainWindow::slotSelectionColorChanged()
{
    if (checkboxColor->checkState()) // Если Чекбокс активен
    {
        chartSettings.chart->changeColor(); // Меняем цвет
        chartSettings.chart->reDrawChart(); // Перерисовываем
    }
    else
    {
        chartSettings.chart->changeColor(); // Меняем цвет
        chartSettings.chart->reDrawChart(); // Перерисовываем
    }
}

// Слот обработки сохранения Диаграммы в PDF
void MainWindow::slotSaveToPdf()
{
    if (isCharCanPrintPDF) // Если можно печатать
    {
        QString savingPath("");

        QFileDialog dialog(this); // Создаём диалог

        dialog.setViewMode(QFileDialog::Detail); // Устанавливаем детальный вид с выбором расширения

        if (dialog.exec())
        {
            savingPath = dialog.selectedFiles().first(); // Получаем из диалога директорию для сохранения
        }

        QPdfWriter writer(savingPath+".pdf"); // Добавляем расширение .pdf

        writer.setCreator("Someone"); // Указываем создателя документа

        writer.setPageSize(QPagedPaintDevice::A4); // Устанавливаем размер страницы

        QPainter painter(&writer);

        chartSettings.chartView->render(&painter); // Печатаем Диаграмму
        painter.end();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText("Отсутствует диаграмма чтобы напечатать");
        messageBox.exec();
    }
}

MainWindow::~MainWindow()
{

}
