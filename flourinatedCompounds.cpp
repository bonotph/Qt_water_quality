
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
  searchButton = new QPushButton(tr("Search"));

  series = new QLineSeries();
  scatterSeries = new QScatterSeries();
  scatterSeries->setMarkerSize(20.0);
  chart = new QChart();
  chartView = new QChartView(chart);

  siteBox->addItems(siteNames);
  compoundBox->addItems(compoundNames);
  
  filterLayout->addWidget(siteBox);
  filterLayout->addWidget(compoundBox);
  filterLayout->addWidget(searchButton);
  mainLayout->addLayout(filterLayout);

  chart->legend()->hide();
  createXAxis();
  mainLayout->addWidget(chartView);

  setLayout(mainLayout);
}

void FlourinatedCompounds::changeChart(QString site, QString compound)
{
  series->clear();
  scatterSeries->clear();

  for (int i = 0; i < model->rowCount(QModelIndex()); i++) {
    QString rowSite = model->data(model->index(i, 1), Qt::DisplayRole).toString();
    QString rowCompound = model->data(model->index(i, 2), Qt::DisplayRole).toString();

    if (rowSite == site && rowCompound == compound) {
      QString timeString = model->data(model->index(i, 0), Qt::DisplayRole).toString();
      double result = model->data(model->index(i, 3), Qt::DisplayRole).toDouble();
      std::cout << timeString.toStdString() << " " << result << std::endl;

      QDateTime time = QDateTime::fromString(timeString, "yyyy-MM-ddThh:mm:ss");
      series->append(time.toMSecsSinceEpoch(), result);
      scatterSeries->append(time.toMSecsSinceEpoch(), result);

      if (result<miny) miny = result;
      if (result>maxy) maxy = result;

      unit = model->data(model->index(i, 4), Qt::DisplayRole).toString();
    }
  }

  // Control output of points in series
  for (const QPointF &point : series->points()) {
      std::cout << "Point(x: " << point.x() << ", y: " << point.y() << ")" << std::endl;
  }

  chart->setTitle(site + " - " + compound);
  
  // Remove existing axes
  for (QAbstractAxis* axis : chart->axes(Qt::Vertical)) {
    chart->removeAxis(axis);
  }

  // Add new Y-axis
  yAxis = new QValueAxis();
  yAxis->setTitleText(tr("Testresult in ") + unit);

  padding = (maxy - miny) * 0.1;
  if (miny == maxy) padding = miny * 0.1;
  yAxis->setRange(miny - padding, maxy + padding);

  std::cout << "miny: " << miny << ", maxy: " << maxy << ", padding: " << padding << std::endl;

  chart->addAxis(yAxis, Qt::AlignLeft);

  if (chart->series().isEmpty()) {
    createColoredRegions();
    chart->addSeries(series);
    chart->addSeries(scatterSeries);
  }

  series->attachAxis(yAxis);
  scatterSeries->attachAxis(yAxis);

  series->attachAxis(xAxis);
  scatterSeries->attachAxis(xAxis);

  chartView->update();
}

void FlourinatedCompounds::createXAxis()
{
  xAxis = new QDateTimeAxis();
  xAxis->setFormat("yyyy-MM-dd");
  xAxis->setTitleText(tr("Time"));

  QDateTime date1 = QDateTime::fromString("2023-12-01", "yyyy-MM-dd");
  QDateTime date2 = QDateTime::fromString("2025-01-31", "yyyy-MM-dd");
  xAxis->setRange(date1, date2);

  chart->addAxis(xAxis, Qt::AlignBottom);
  series->attachAxis(xAxis);
  scatterSeries->attachAxis(xAxis);
}

void FlourinatedCompounds::search()
{
  site = siteBox->currentText();
  compound = compoundBox->currentText();
  changeChart(site, compound);
}

void FlourinatedCompounds::showPointTooltip(const QPointF& point, bool state)
{
  if (state) {
    QString tooltipText = QString("X: %1\nY: %2 \n Flourinated compounds can be harmful to the environment.\nThey can pose real threats and should be observed carefully")
        .arg(QDateTime::fromMSecsSinceEpoch(point.x()).toString("yyyy-MM-dd"))
        .arg(point.y());
    QToolTip::showText(QCursor::pos(), tooltipText);
  } else { // Hide text when mouse is moved away
    QToolTip::hideText();
  }
}

void FlourinatedCompounds::retranslateUi()
{
    searchButton->setText(tr("Search"));
}

void FlourinatedCompounds::makeConnections()
{
  connect(searchButton, SIGNAL(clicked()), this, SLOT(search()));
  connect(scatterSeries, &QScatterSeries::hovered, this, &FlourinatedCompounds::showPointTooltip);
}

void FlourinatedCompounds::createColoredRegions()
{
  // This is only a front-end implementation of the compliance regions for measurements
  // These do not have any correlation to the actual data yet
  // We tried to find colours that would work for colourblind or visually impaired users as well
  // However, we are not such users and we don´t know anyone who is, so we couldn´t test it
    double greenTop = miny + (maxy - miny) / 3;
    double amberTop = miny + 2 * (maxy - miny) / 3;

    // Green region
    QLineSeries* greenLineTop = new QLineSeries();
    greenLineTop->append(xAxis->min().toMSecsSinceEpoch(), greenTop);
    greenLineTop->append(xAxis->max().toMSecsSinceEpoch(), greenTop);

    QLineSeries* greenLineBottom = new QLineSeries();
    greenLineBottom->append(xAxis->min().toMSecsSinceEpoch(), (miny - padding));
    greenLineBottom->append(xAxis->max().toMSecsSinceEpoch(), (miny - padding));

    QAreaSeries* greenArea = new QAreaSeries(greenLineBottom, greenLineTop);
    greenArea->setBrush(QBrush(QColor(150, 230, 170)));

    // Amber region
    QLineSeries* amberLineTop = new QLineSeries();
    amberLineTop->append(xAxis->min().toMSecsSinceEpoch(), amberTop);
    amberLineTop->append(xAxis->max().toMSecsSinceEpoch(), amberTop);

    QLineSeries* amberLineBottom = new QLineSeries();
    amberLineBottom->append(xAxis->min().toMSecsSinceEpoch(), greenTop);
    amberLineBottom->append(xAxis->max().toMSecsSinceEpoch(), greenTop);

    QAreaSeries* amberArea = new QAreaSeries(amberLineBottom, amberLineTop);
    amberArea->setBrush(QBrush(QColor(250, 245, 200)));

    // Red region (top)
    QLineSeries* redLineTop = new QLineSeries();
    redLineTop->append(xAxis->min().toMSecsSinceEpoch(), (maxy + padding));
    redLineTop->append(xAxis->max().toMSecsSinceEpoch(), (maxy + padding));

    QLineSeries* redLineBottom = new QLineSeries();
    redLineBottom->append(xAxis->min().toMSecsSinceEpoch(), amberTop);
    redLineBottom->append(xAxis->max().toMSecsSinceEpoch(), amberTop);

    QAreaSeries* redArea = new QAreaSeries(redLineBottom, redLineTop);
    redArea->setBrush(QBrush(QColor(220, 120, 120)));

    // Add areas to the chart
    chart->addSeries(greenArea);
    chart->addSeries(amberArea);
    chart->addSeries(redArea);

    // Attach axes
    greenArea->attachAxis(xAxis);
    greenArea->attachAxis(yAxis);
    amberArea->attachAxis(xAxis);
    amberArea->attachAxis(yAxis);
    redArea->attachAxis(xAxis);
    redArea->attachAxis(yAxis);
}

