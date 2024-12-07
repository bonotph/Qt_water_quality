#include "complianceItem.hpp"
#include <QtCharts>
ComplianceDashboardItem::ComplianceDashboardItem(
    const QVector<QString>& locations,
    const QString& pollutants,
    double thresholds,
    const QVector<double>& values,
    const QVector<bool>& compliances,
    QWidget *parent
    ) : QWidget(parent),
    itemLocation(locations),
    itemPollutant(pollutants),
    itemThreshold(thresholds),
    itemValue(values),
    itemIsComply(compliances){
    createWidgets();
}


void ComplianceDashboardItem::createWidgets() {
    itemLayout = new QVBoxLayout(this);

    // pollutant name
    itemNameLabel = new QLabel(itemPollutant, this);
    itemNameLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    itemLayout->addWidget(itemNameLabel);

    //compliance status
    m_statusLabel = new QLabel(this);
    QString status = determineComplianceStatus(itemValue, itemThreshold, itemIsComply);
    complianceStatus = status=="Compliant" || status=="Caution";
    m_statusLabel->setText(QString("Compliance Status: %1").arg(status));
    m_statusLabel->setStyleSheet(QString(
                                     "font-weight: bold; padding: 5px; border-radius: 5px; "
                                     "background-color: %1; color: white;"
                                     ).arg(getComplianceColor(status).name()));
    itemLayout->addWidget(m_statusLabel);

    // compliance Charts
    createLineChart();
    createGroupedBarChart();
    createSummaryTable();
    createGraphs();

    setLayout(itemLayout);
}

void ComplianceDashboardItem::updateData(
    const QVector<QString>& locations,
    double thresholds,
    const QVector<double>& values,
    const QVector<bool>& compliances
    ) {
    itemLocation = locations;
    itemThreshold = thresholds;
    itemValue = values;

    // recreate with new data
    QLayoutItem *child;
    while ((child = itemLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    createWidgets();
}

QString ComplianceDashboardItem::determineComplianceStatus(
    const QVector<double>& measurements,
    double safeThreshold,
    const QVector<bool>& isComply
    ) {
    int exceedanceCount = std::count_if(measurements.begin(), measurements.end(),
                                        [safeThreshold](double val) { return val > safeThreshold; }); //compare with threshold
    int statusCount = std::count_if(isComply.begin(), isComply.end(),
                                    [safeThreshold](double val) { return val == true; }); // compare with compliance data
    double exceedancePercentage = (double)exceedanceCount / measurements.size() * 100;
    double statusPercentage = statusCount/isComply.size() * 100;
    if (exceedancePercentage <= 10 && statusPercentage >= 50) return "Compliant";
    if (exceedancePercentage <= 25 && statusPercentage >= 50) return "Caution";
    return "Non-Compliant";
}


QColor ComplianceDashboardItem::getComplianceColor(const QString& status) {
    if (status == "Compliant") return QColor(0, 180, 0);     // green
    if (status == "Caution") return QColor(255, 165, 0);     // yellow
    return QColor(255, 0, 0);                               // red
}


QChartView* ComplianceDashboardItem::createLineChart() {
    QChart *chart = new QChart();
    chart->setTitle(QString("%1 Levels Across Locations").arg(itemPollutant));

    QLineSeries *series = new QLineSeries();
    series->setName(itemPollutant);

    for (int i = 0; i < itemValue.size(); ++i) {
        series->append(i, itemValue[i]);
    }

    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(itemLocation);
    axisX->setLabelsAngle(-45); 

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("ug/l");
    axisY->setMin(0);
    axisY->setMax(*std::max_element(itemValue.begin(), itemValue.end()) * 1.2);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Use CustomChartView instead of QChartView
    CustomChartView *chartView = new CustomChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedHeight(300);

    return chartView;
}


QChartView* ComplianceDashboardItem::createGroupedBarChart() {
    QChart *chart = new QChart();
    chart->setTitle(QString("%1 Measurements Comparison").arg(itemPollutant));

    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    QBarSet *measurementSet = new QBarSet("Measurement");

    QVector<double> scaledMeasurements;
    for (double value : itemValue) {
        scaledMeasurements.append(value); 
    }

    // Determine if all values are the same
    bool allSame = std::all_of(scaledMeasurements.begin(), scaledMeasurements.end(),
                               [scaledMeasurements](double val) { return val == scaledMeasurements[0]; });

    if (allSame) {
        // All values are identical
        *measurementSet << scaledMeasurements[0];
        chart->setTitle(QString("%1 Measurements Comparison (All Values Same)").arg(itemPollutant));
        chart->addSeries(series);

        QBarCategoryAxis *axisY = new QBarCategoryAxis();
        axisY->append("All Locations");

        QValueAxis *axisX = new QValueAxis();
        axisX->setTitleText("ug/l");
        axisX->setMin(0);
        axisX->setMax(scaledMeasurements[0] * 1.2); 

        chart->addAxis(axisY, Qt::AlignLeft);
        chart->addAxis(axisX, Qt::AlignBottom);

        series->append(measurementSet);
        series->attachAxis(axisY);
        series->attachAxis(axisX);

    } else {
        // Select the largest and smallest values
        double maxValue = *std::max_element(scaledMeasurements.begin(), scaledMeasurements.end());
        double minValue = *std::min_element(scaledMeasurements.begin(), scaledMeasurements.end());

        int maxIndex = scaledMeasurements.indexOf(maxValue);
        int minIndex = scaledMeasurements.indexOf(minValue);

        QVector<QString> filteredLocations;
        filteredLocations.append(itemLocation[maxIndex]);
        filteredLocations.append(itemLocation[minIndex]);

        *measurementSet << maxValue << minValue;

        series->append(measurementSet);
        chart->addSeries(series);

        QBarCategoryAxis *axisY = new QBarCategoryAxis();
        axisY->append(filteredLocations);

        QValueAxis *axisX = new QValueAxis();
        axisX->setTitleText("ug/l");
        axisX->setMin(0);
        axisX->setMax(maxValue * 1.2); // Adjusted for scaling

        chart->addAxis(axisY, Qt::AlignLeft);
        chart->addAxis(axisX, Qt::AlignBottom);

        series->attachAxis(axisY);
        series->attachAxis(axisX);
    }

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedHeight(300);

    return chartView;
}


QTableWidget* ComplianceDashboardItem::createSummaryTable() {
    QTableWidget *table = new QTableWidget(itemLocation.size() + 1, 4);
    table->setHorizontalHeaderLabels({"Location", "Measurement", "Threshold", "Compliance"});

    // Header row formatting
    QFont headerFont = table->horizontalHeaderItem(0)->font();
    headerFont.setBold(true);
    for (int col = 0; col < 4; ++col) {
        table->horizontalHeaderItem(col)->setFont(headerFont);
    }

    // Populate table with location data
    for (int i = 0; i < itemLocation.size(); ++i) {
        QTableWidgetItem *locationItem = new QTableWidgetItem(itemLocation[i]);
        QTableWidgetItem *measurementItem = new QTableWidgetItem(QString::number(itemValue[i]));
        QTableWidgetItem *thresholdItem = new QTableWidgetItem(QString::number(itemThreshold));
        QTableWidgetItem *complianceItem = new QTableWidgetItem(itemIsComply[i] ? "Compliant" : "Non-Compliant");
        
        // Color code compliance
        complianceItem->setBackground(itemIsComply[i] ? QColor(200, 255, 200) : QColor(255, 200, 200));

        table->setItem(i, 0, locationItem);
        table->setItem(i, 1, measurementItem);
        table->setItem(i, 2, thresholdItem);
        table->setItem(i, 3, complianceItem);
    }

    table->resizeColumnsToContents();
    return table;
}

void ComplianceDashboardItem::createGraphs() {
    // Create a tab widget to host different views
    QTabWidget *tabWidget = new QTabWidget(this);

    // 1. Line Chart View (Scrollable)
    QChartView *lineChartView = createLineChart();
    tabWidget->addTab(lineChartView, "Trend Chart");

    // 2. Bar Chart View 
    QChartView *barChartView = createGroupedBarChart();
    tabWidget->addTab(barChartView, "Comparison Chart");

    // 3. Summary Table View
    QTableWidget *summaryTable = createSummaryTable();
    tabWidget->addTab(summaryTable, "Data Summary");

    itemLayout->addWidget(tabWidget);
}


/*
search?
sorting
location and pollutant filter
pop up
loading time
*/