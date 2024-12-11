#include "pollutantOverview.hpp"
#include <QDateTime>
#include <QDebug>

PollutantOverview::PollutantOverview(SampleModel* sharedModel, QWidget* parent)
    : QWidget(parent), model(sharedModel)
{
    series = new QLineSeries();
    chart = new QChart();
    chartView = new QChartView(chart);

    // Call functions to set up the widgets, layouts, and connections
    createWidgets();
    createLayouts();
    makeConnections();

    // Initial graph setup
    chart->setTitle("Pollutant Trends");
    chart->addSeries(series);
    chart->createDefaultAxes();
}

void PollutantOverview::createWidgets()
{
    pollutantSearchBox = new QComboBox();
    pollutantSearchBox->addItems(model->getUniquePollutants());  // Assuming this method returns pollutant names.

    searchButton = new QPushButton("&Search");
}

void PollutantOverview::createLayouts()
{
    searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search Pollutant:"));
    searchLayout->addWidget(pollutantSearchBox);
    searchLayout->addWidget(searchButton);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(chartView);

    setLayout(mainLayout);
}

void PollutantOverview::makeConnections()
{
    connect(searchButton, &QPushButton::clicked, this, &PollutantOverview::onSearchClicked);
}

void PollutantOverview::onSearchClicked()
{
    QString selectedPollutant = pollutantSearchBox->currentText();
    updateGraph(selectedPollutant);
}

void PollutantOverview::updateGraph(const QString& pollutantName)
{
    series->clear();

    // Iterate through the model data to find matching pollutant entries
    for (int i = 0; i < model->rowCount(QModelIndex()); ++i)
    {
        QString pollutant = model->data(model->index(i, 2), Qt::DisplayRole).toString();  // Column 2: determinand.label (assumed)
        if (pollutant == pollutantName)
        {
            QString dateTimeStr = model->data(model->index(i, 0), Qt::DisplayRole).toString();  // Column 0: sample.sampleDateTime (assumed)
            double result = model->data(model->index(i, 3), Qt::DisplayRole).toDouble();         // Column 3: result (assumed)

            QDateTime dateTime = QDateTime::fromString(dateTimeStr, Qt::ISODate);
            series->append(dateTime.toMSecsSinceEpoch(), result);
        }
    }

    // Update the chart's title and create default axes
    chart->setTitle(pollutantName + " Trends");
    chart->createDefaultAxes();
}

void PollutantOverview::retranslateUi()
{
    searchButton->setText(tr("&Search"));
    // probably missing widgets
}