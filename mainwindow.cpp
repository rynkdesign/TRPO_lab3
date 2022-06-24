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
    QChartView *chartView;
    auto themeWidget = new ThemeWidget();
    QChart *chartBar =  themeWidget->createBarChart(7);
    chartView = new QChartView(chartBar);

    // Объявляем Buttons
    auto directoryButton = new QPushButton ("Открыть папку");
    auto printChartButton = new QPushButton ("Печать графика");
    auto checkboxBlackWhiteChart = new QCheckBox("Черно-белый график");
    auto boxLabel = new QLabel("Выберите тип диаграммы");
    auto chartTypeCombobox = new QComboBox(); // Выбор типа графика
    chartTypeCombobox->insertItem(1, QString("BarChart"));
    chartTypeCombobox->insertItem(2, QString("PieChart"));

    // Объявляем Layouts
    auto horizontalLayout=new QHBoxLayout(this);
    auto verticalLeftLayout = new QVBoxLayout();
    auto verticalRightLayout = new QVBoxLayout();
    auto chartLayout = new QHBoxLayout(); // Layout для кнопок над графиком

    horizontalLayout->addLayout(verticalLeftLayout);
    horizontalLayout->addLayout(verticalRightLayout);
    verticalRightLayout->addLayout(chartLayout);
    verticalLeftLayout->addWidget(tableView);
    verticalRightLayout->addWidget(chartView);
    verticalLeftLayout->addWidget(directoryButton); // Кнопка "Открыть папку"

    // Buttons над графиком
    chartLayout->addWidget(boxLabel); // Label над графиком
    chartLayout->addWidget(chartTypeCombobox); // Тип графика
    chartLayout->addWidget(checkboxBlackWhiteChart); // Ч-б
    chartLayout->addWidget(printChartButton); // Печать в PDF

    // Slots
    connect(directoryButton,&QPushButton::clicked,this,&MainWindow::slotChooseDirectory);
}


// Выбор конкретного файла
void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    //Q_UNUSED(selected);
    Q_UNUSED(deselected);
    //QModelIndex index = treeView->selectionModel()->currentIndex();
    QModelIndexList indexs =  selected.indexes();
    QString filePath = "";

    // Размещаем инфо в statusbar относительно выделенного модельного индекса

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        //filePath = dirModel->filePath(ix);
        //this->statusBar()->showMessage("Выбранный путь : " + dirModel->filePath(indexs.constFirst()));
    }

    //TODO: !!!!!
    /*
    Тут простейшая обработка ширины первого столбца относительно длины названия папки.
    Это для удобства, что бы при выборе папки имя полностью отображалась в  первом столбце.
    Требуется доработка(переработка).
    */
    int length = 200;
    int dx = 30;

    //if (dirModel->fileName(index).length() * dx > length) {
    //    length = length + dirModel->fileName(index).length() * dx;
    //    qDebug() << "r = " << index.row() << "c = " << index.column() << dirModel->fileName(index) << dirModel->fileInfo(
    //                 index).size();

    //}

    //treeView->header()->resizeSection(index.column(), length + dirModel->fileName(index).length());
   // tableView->setRootIndex(fileModel->setRootPath(filePath));
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

MainWindow::~MainWindow()
{

}
