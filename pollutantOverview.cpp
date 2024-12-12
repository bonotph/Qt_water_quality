#include "pollutantOverview.hpp"
#include <QDateTime>
#include <QDebug>
#include <QMap>
#include <QVector>
#include <numeric>

PollutantOverview::PollutantOverview(SampleModel* sharedModel, QWidget* parent)
    : QWidget(parent), model(sharedModel), series(nullptr), chart(nullptr), chartView(nullptr), xAxis(nullptr), yAxis(nullptr)
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
    chart->legend()->hide();
    createXAxis();
}

void PollutantOverview::createWidgets()
{
    pollutantSearchBox = new QComboBox();
    if (model) {
        pollutantSearchBox->addItems(model->getUniquePollutants());  // Assuming this method returns pollutant names.
    }

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
    if (!model || pollutantName.isEmpty()) {
        qWarning() << "Invalid model or pollutant name";
        return;
    }

    series->clear();  // Clear existing series data

    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    // Step 1: Group data by week
    QMap<int, QVector<double>> weeklyData; // Maps week number in ISO year to values
    QMap<int, int> weeklyYear;            // Tracks the year for each week number

    for (int i = 0; i < model->rowCount(QModelIndex()); ++i)
    {
        QString pollutant = model->data(model->index(i, 2), Qt::DisplayRole).toString();
        if (pollutant == pollutantName)
        {
            QString dateTimeStr = model->data(model->index(i, 0), Qt::DisplayRole).toString();
            double result = model->data(model->index(i, 3), Qt::DisplayRole).toDouble();
            std::cout << dateTimeStr.toStdString() << " " << result << std::endl;

            QDateTime dateTime = QDateTime::fromString(dateTimeStr, Qt::ISODate);
            if (!dateTime.isValid())
                continue;

            // Get week number and year using ISO week system
            int weekNumber = dateTime.date().weekNumber();
            int year = dateTime.date().year();

            int combinedKey = year * 100 + weekNumber; // Combine year and week number into a unique key
            weeklyData[combinedKey].append(result);
            weeklyYear[combinedKey] = year;
        }
    }

    // Step 2: Calculate weekly averages and prepare data for plotting
    QDateTime minDateTime;
    QDateTime maxDateTime;

    for (auto it = weeklyData.begin(); it != weeklyData.end(); ++it)
    {
        int combinedKey = it.key();
        QVector<double> values = it.value();

        // Calculate average for the week
        double total = std::accumulate(values.begin(), values.end(), 0.0);
        double average = total / values.size();

        // Extract year and week number from combinedKey
        int year = weeklyYear[combinedKey];
        int weekNumber = combinedKey % 100;

        // Calculate the start date of the ISO week
        QDate firstDayOfYear(year, 1, 4); // ISO weeks start around Jan 4
        QDate startOfWeek = firstDayOfYear.addDays((weekNumber - 1) * 7 - firstDayOfYear.dayOfWeek() + 1);

        QDateTime dateTime(startOfWeek, QTime(0, 0));
        if (minDateTime.isNull() || dateTime < minDateTime) {
            minDateTime = dateTime;
        }
        if (maxDateTime.isNull() || dateTime > maxDateTime) {
            maxDateTime = dateTime;
        }

        // Add the point to the series
        series->append(dateTime.toMSecsSinceEpoch(), average);
        std::cout << "Week: " << year << "-W" << weekNumber << ", Average: " << average << std::endl;

        minY = qMin(minY, average);
        maxY = qMax(maxY, average);
    }

    // Step 3: Adjust the chart
    double padding = (maxY - minY) * 0.1;
    if (minY == maxY) padding = minY * 0.1; // Handle edge case

    if (!yAxis) {
        yAxis = new QValueAxis();
        chart->addAxis(yAxis, Qt::AlignLeft);
    }

    yAxis->setRange(minY - padding, maxY + padding);
    yAxis->setTitleText(tr("Pollutant Levels"));

    if (!chart->series().contains(series)) {
        chart->addSeries(series);
    }

    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    chart->setTitle(pollutantName + " Weekly Averages");
    chartView->update();
}


void PollutantOverview::retranslateUi()
{
    searchButton->setText(tr("&Search"));
}

void PollutantOverview::createXAxis()
{
  xAxis = new QDateTimeAxis();
  xAxis->setFormat("yyyy-MM-dd");
  xAxis->setTitleText(tr("Time"));

  QDateTime date1 = QDateTime::fromString("2023-12-01", "yyyy-MM-dd");
  QDateTime date2 = QDateTime::fromString("2025-01-31", "yyyy-MM-dd");
  xAxis->setRange(date1, date2);

  chart->addAxis(xAxis, Qt::AlignBottom);
  series->attachAxis(xAxis);
}
