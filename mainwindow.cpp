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
    verticalLeftLayout->addWidget(directoryButton); // Кнопка "Открыть папку"

    // Buttons над графиком
    chartLayout->addWidget(boxLabel); // Label над графиком
    chartLayout->addWidget(boxType); // Тип графика
    chartLayout->addWidget(checkboxColor); // Ч-б
    chartLayout->addWidget(printChartButton); // Печать в PDF

    QItemSelectionModel *selectionModel = tableView->selectionModel();

    // Slots
    connect(directoryButton,&QPushButton::clicked,this,&MainWindow::slotChooseDirectory); // Кнопка выбора папки
    connect(boxType,SIGNAL(currentTextChanged(const QString&)),this,SLOT(slotSelectionComboboxChanged())); // Выбор типа графика
    connect(checkboxColor, SIGNAL(toggled(bool)), this, SLOT(slotSelectionColorChanged())); // Выбор Ч-Б
    // Выбор конкретного файла
    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),this,SLOT(slotSelectionChanged(const QItemSelection &, const QItemSelection &)));
    connect(printChartButton,SIGNAL(clicked()), this, SLOT(slotSaveToPdf()));

}

// Выбор конкретного файла
void MainWindow::slotSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexs =  selected.indexes();
    if (indexs.count() < 1)
    {
        return;
    }

    QString filePath{""}; // Полный путь к файлу
    filePath = fileModel->filePath(indexs.constFirst());

    if (!(filePath.endsWith(".sqlite") || filePath.endsWith(".json")))
    {
        isCharCanPrintPDF = false;
        QMessageBox messageBox;
        messageBox.setText("Выберит файл формата .sqlite или .json");
        messageBox.exec();
        return;
    }

    // Пока что только sqlite
    if (filePath.endsWith(".sqlite"))
    {
        auto data = ChartDataSqlite{}.getData(filePath);//sql

        if (data.isEmpty())
        {
            QMessageBox messageBox;
            messageBox.setText("Файл пустой");
            messageBox.exec();
            return;
        }

        chartSettings.chart->drawChart(data);
        chartSettings.chartView->setChart(chartSettings.chart->getChart());
    }
    if (filePath.endsWith(".json"))
    {
        //Json
    }
    isCharCanPrintPDF = true;

}

// Диалог для открытия папки
void MainWindow::slotChooseDirectory()
{
    QFileDialog dialog{this};
    dialog.setFileMode(QFileDialog::Directory);
    if ( dialog.exec() )
    {
        homePath = dialog.selectedFiles().first();
    }
    tableView->setRootIndex(fileModel->setRootPath(homePath));
}

// Функция выбора Графика
void MainWindow::slotSelectionComboboxChanged()
{
    QString chartType = boxType->currentText();

    if(chartType == "PieChart")
    {
        IOCContainer::instance().RegisterInstance<IChartDrawing, pieChartDrawing>();
        chartSettings.chart->reDrawChart();
        return;
    }
    else if (chartType == "BarChart")
    {
        IOCContainer::instance().RegisterInstance<IChartDrawing, barChartDrawing>();
        chartSettings.chart->reDrawChart();
        return;
    }
}

void MainWindow::slotSelectionColorChanged()
{
    if (checkboxColor->checkState())
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

void MainWindow::slotSaveToPdf()
{
    if (isCharCanPrintPDF)
    {
        QString savingPath("");

        QFileDialog dialog(this);

        dialog.setViewMode(QFileDialog::Detail); // Устанавливаем детальный вид с выбором расширения

        if (dialog.exec())
        {
            savingPath = dialog.selectedFiles().first();
        }

        QPdfWriter writer(savingPath+".pdf");

        writer.setCreator("Someone");//Указываем создателя документа

        writer.setPageSize(QPagedPaintDevice::A4);//Устанавливаем размер страницы

        QPainter painter(&writer);

        chartSettings.chartView->render(&painter);
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
