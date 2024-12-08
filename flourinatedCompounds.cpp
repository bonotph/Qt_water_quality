
#include <QtWidgets>
#include <QtCharts>
#include <QPointF>

#include "flourinatedCompounds.hpp"

FlourinatedCompounds::FlourinatedCompounds(SampleModel* sharedModel)
{
  model = sharedModel;

  createWidgets();
  makeConnections();
}

void FlourinatedCompounds::createWidgets()
{
  filterLayout = new QHBoxLayout();
  mainLayout = new QVBoxLayout();

  siteBox = new QComboBox();
  compoundBox = new QComboBox();
  searchButton = new QPushButton("Search");

  series = new QLineSeries();
  chart = new QChart();
  chartView = new QChartView(chart);

  siteBox->addItems(siteNames);
  compoundBox->addItems(compoundNames);
  
  filterLayout->addWidget(siteBox);
  filterLayout->addWidget(compoundBox);
  filterLayout->addWidget(searchButton);
  mainLayout->addLayout(filterLayout);

  series->setPointsVisible(true);
  chart->legend()->hide();
  createXAxis();
  mainLayout->addWidget(chartView);

  setLayout(mainLayout);
}

void FlourinatedCompounds::changeChart(QString site, QString compound)
{
  double miny = 100;
  double maxy = 0;
  
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

      if (result<miny) miny = result;
      if (result>maxy) maxy = result;
    }
  }

  // Control output of points in series
  for (const QPointF &point : series->points()) {
      std::cout << "Point(x: " << point.x() << ", y: " << point.y() << ")" << std::endl;
  }

  if (chart->series().isEmpty()) {
    chart->addSeries(series);
  }

  chart->setTitle(site + " - " + compound);
  
  // Remove existing axes
  for (QAbstractAxis* axis : chart->axes(Qt::Vertical)) {
    chart->removeAxis(axis);
  }

  // Add new Y-axis
  yAxis = new QValueAxis();
  yAxis->setTitleText("Value");

  double padding = (maxy - miny) * 0.1;
  if (miny == maxy) padding = miny * 0.1;
  yAxis->setRange(miny - padding, maxy + padding);

  chart->addAxis(yAxis, Qt::AlignLeft);
  series->attachAxis(yAxis);
}

void FlourinatedCompounds::createXAxis()
{
  QDateTimeAxis* xAxis = new QDateTimeAxis();
  xAxis->setFormat("yyyy-MM-dd");
  xAxis->setTitleText("Time");

  QDateTime date1 = QDateTime::fromString("2024-01-01", "yyyy-MM-dd");
  QDateTime date2 = QDateTime::fromString("2024-12-31", "yyyy-MM-dd");
  xAxis->setRange(date1, date2);

  chart->addAxis(xAxis, Qt::AlignBottom);
  series->attachAxis(xAxis);
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