
#include <QtWidgets>
#include <QtCharts>

#include "flourinatedCompounds.hpp"

FlourinatedCompounds::FlourinatedCompounds(SampleModel* sharedModel)
{
  model = sharedModel;
  std::cout << "Model address: " << model << std::endl;

  series = new QLineSeries();
  chart = new QChart();
  chartView = new QChartView(chart);

  createWidgets();
  makeConnections();

  addChart();
}

void FlourinatedCompounds::createWidgets()
{
  filterLayout = new QHBoxLayout();
  siteBox = new QComboBox();
  compoundBox = new QComboBox();
  searchButton = new QPushButton("Search");

  siteBox->addItems(siteNames);

  compoundBox->addItems(compoundNames);
  
  filterLayout->addWidget(siteBox);
  filterLayout->addWidget(compoundBox);
  filterLayout->addWidget(searchButton);

  mainLayout = new QVBoxLayout();
  mainLayout->addLayout(filterLayout);

  setLayout(mainLayout);
}

void FlourinatedCompounds::addChart()
{
  
  series->append(0,5);
  series->append(1,6);
  series->append(2,7);
  series->append(3,8);
  series->append(4,9);

  chart->legend()->hide();
  chart->addSeries(series);
  chart->createDefaultAxes();
  chart->setTitle("Testing Charts");

  chartView->setRenderHint(QPainter::Antialiasing); 

  mainLayout->addWidget(chartView);

}

void FlourinatedCompounds::changeChart(QString site, QString compound)
{
    series->clear();

    for (int i = 0; i < model->rowCount(QModelIndex()); i++) {
        QString rowSite = model->data(model->index(i, 1), Qt::DisplayRole).toString();
        QString rowCompound = model->data(model->index(i, 2), Qt::DisplayRole).toString();

        if (rowSite == site && rowCompound == compound) {
            QString timeString = model->data(model->index(i, 0), Qt::DisplayRole).toString();
            double result = model->data(model->index(i, 3), Qt::DisplayRole).toDouble();
            std::cout << timeString.toStdString() << " " << result << std::endl;

            QDateTime time = QDateTime::fromString(timeString, "yyyy-MM-ddThh:mm:ss");
            series->append(time.toMSecsSinceEpoch(), result);
        }
    }

    chart->setTitle(site + " - " + compound);

    /*QDateTimeAxis* xAxis = new QDateTimeAxis();
    xAxis->setFormat("yyyy-MM-dd");
    xAxis->setTitleText("Time");
    chart->setAxisX(xAxis, series);*/
}


void FlourinatedCompounds::search()
{
  site = siteBox->currentText();
  compound = compoundBox->currentText();
  changeChart(site, compound);
}

void FlourinatedCompounds::makeConnections()
{
  connect(searchButton, SIGNAL(clicked()), this, SLOT(search()));
}