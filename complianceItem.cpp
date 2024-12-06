#include "complianceItem.hpp"
#include <QVBoxLayout>
#include <QtCharts>

//constructor
ComplianceDashboardItem::ComplianceDashboardItem(
    const QVector<QString>& locations,
    const QString& pollutantName,
    double safeThreshold,
    const QVector<double>& measurements,
    const QVector<bool>& isCompliance,
    QWidget *parent
    ) : QWidget(parent),
    m_location(locations),
    m_pollutantName(pollutantName),
    m_safeThreshold(safeThreshold),
    m_measurements(measurements),
    m_isComply(isCompliance){
    createMainWidget();
}


void ComplianceDashboardItem::createMainWidget() {
    m_layout = new QVBoxLayout(this);

    // Pollutant Name
    m_nameLabel = new QLabel(m_pollutantName, this);
    m_nameLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    m_layout->addWidget(m_nameLabel);

    // Compliance Status
    m_statusLabel = new QLabel(this);
    QString status = determineComplianceStatus(m_measurements, m_safeThreshold, m_isComply);
    ComplianceStatus = status=="Compliant" || status=="Caution";
    m_statusLabel->setText(QString("Compliance Status: %1").arg(status));
    m_statusLabel->setStyleSheet(QString(
                                     "font-weight: bold; padding: 5px; border-radius: 5px; "
                                     "background-color: %1; color: white;"
                                     ).arg(getComplianceColor(status).name()));
    m_layout->addWidget(m_statusLabel);

    // Create Compliance Chart
    createScrollableLineChart();
    createGroupedBarChart();
    createSummaryTable();
    createComplianceVisualization();

    setLayout(m_layout);
}

void ComplianceDashboardItem::createComplianceVisualization() {
    // Create a tab widget to host different views
    QTabWidget *tabWidget = new QTabWidget(this);

    // 1. Line Chart View (Scrollable)
    QChartView *lineChartView = createScrollableLineChart();
    tabWidget->addTab(lineChartView, "Trend Chart");

    // 2. Bar Chart View 
    QChartView *barChartView = createGroupedBarChart();
    tabWidget->addTab(barChartView, "Comparison Chart");

    // 3. Summary Table View
    QTableWidget *summaryTable = createSummaryTable();
    tabWidget->addTab(summaryTable, "Data Summary");

    m_layout->addWidget(tabWidget);
}

QChartView* ComplianceDashboardItem::createScrollableLineChart() {
    QChart *chart = new QChart();
    chart->setTitle(QString("%1 Levels Across Locations").arg(m_pollutantName));

    QLineSeries *series = new QLineSeries();
    series->setName(m_pollutantName);

    for (int i = 0; i < m_measurements.size(); ++i) {
        series->append(i, m_measurements[i]);
    }

    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(m_location);
    axisX->setLabelsAngle(-45);  // Rotate labels for better readability

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("ug/l");
    axisY->setMin(0);
    axisY->setMax(*std::max_element(m_measurements.begin(), m_measurements.end()) * 1.2);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedHeight(300);

    return chartView;
}


QChartView* ComplianceDashboardItem::createGroupedBarChart() {
    QChart *chart = new QChart();
    chart->setTitle(QString("%1 Measurements Comparison").arg(m_pollutantName));

    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    QBarSet *measurementSet = new QBarSet("Measurement");

    QVector<double> scaledMeasurements;
    for (double value : m_measurements) {
        scaledMeasurements.append(value); 
    }

    // Determine if all values are the same
    bool allSame = std::all_of(scaledMeasurements.begin(), scaledMeasurements.end(),
                               [scaledMeasurements](double val) { return val == scaledMeasurements[0]; });

    if (allSame) {
        // All values are identical
        *measurementSet << scaledMeasurements[0];
        chart->setTitle(QString("%1 Measurements Comparison (All Values Same)").arg(m_pollutantName));
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
        filteredLocations.append(m_location[maxIndex]);
        filteredLocations.append(m_location[minIndex]);

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
    QTableWidget *table = new QTableWidget(m_location.size() + 1, 4);
    table->setHorizontalHeaderLabels({"Location", "Measurement", "Threshold", "Compliance"});

    // Header row formatting
    QFont headerFont = table->horizontalHeaderItem(0)->font();
    headerFont.setBold(true);
    for (int col = 0; col < 4; ++col) {
        table->horizontalHeaderItem(col)->setFont(headerFont);
    }

    // Populate table with location data
    for (int i = 0; i < m_location.size(); ++i) {
        QTableWidgetItem *locationItem = new QTableWidgetItem(m_location[i]);
        QTableWidgetItem *measurementItem = new QTableWidgetItem(QString::number(m_measurements[i]));
        QTableWidgetItem *thresholdItem = new QTableWidgetItem(QString::number(m_safeThreshold));
        QTableWidgetItem *complianceItem = new QTableWidgetItem(m_isComply[i] ? "Compliant" : "Non-Compliant");
        
        // Color code compliance
        complianceItem->setBackground(m_isComply[i] ? QColor(200, 255, 200) : QColor(255, 200, 200));

        table->setItem(i, 0, locationItem);
        table->setItem(i, 1, measurementItem);
        table->setItem(i, 2, thresholdItem);
        table->setItem(i, 3, complianceItem);
    }

    table->resizeColumnsToContents();
    return table;
}

void ComplianceDashboardItem::updateData(
    const QVector<QString>& locations,
    double safeThreshold,
    const QVector<double>& measurements,
    const QVector<bool>& isCompliance
    ) {
    m_location = locations; // Update locations
    m_safeThreshold = safeThreshold;
    m_measurements = measurements;

    // Recreate UI with new data
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    createMainWidget();
}

QString ComplianceDashboardItem::determineComplianceStatus(
    const QVector<double>& measurements,
    double safeThreshold,
    const QVector<bool>& isComply
    ) {
    int exceedanceCount = std::count_if(measurements.begin(), measurements.end(),
                                        [safeThreshold](double val) { return val > safeThreshold; });
    int statusCount = std::count_if(isComply.begin(), isComply.end(),
                                    [safeThreshold](double val) { return val == true; });
    double exceedancePercentage = (double)exceedanceCount / measurements.size() * 100;
    double statusPercentage = statusCount/isComply.size() * 100;
    if (exceedancePercentage <= 10 && statusPercentage >= 50) return "Compliant";
    if (exceedancePercentage <= 25 && statusPercentage >= 50) return "Caution";
    return "Non-Compliant";
}

QColor ComplianceDashboardItem::getComplianceColor(const QString& status) {
    if (status == "Compliant") return QColor(0, 180, 0);     // Green
    if (status == "Caution") return QColor(255, 165, 0);     // Orange
    return QColor(255, 0, 0);                               // Red
}