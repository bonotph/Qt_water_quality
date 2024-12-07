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
    itemLayout = new QHBoxLayout(this);
    graphLayout = new QVBoxLayout();

    // pollutant name
    itemNameLabel = new QLabel(itemPollutant, this);
    itemNameLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    graphLayout->addWidget(itemNameLabel);

    //compliance status
    statusLabel = new QLabel(this);
    QList<QString> status = determineComplianceStatus(itemValue, itemThreshold, itemIsComply);
    complianceStatus = status[0]=="Compliant" || status[0]=="Caution";
    statusLabel->setText(QString("Compliance Status: %1").arg(status[0]));
    statusLabel->setStyleSheet(QString(
                                     "font-weight: bold; padding: 5px; border-radius: 5px; "
                                     "background-color: %1; color: white;"
                                     ).arg(getComplianceColor(status[0]).name()));
    graphLayout->addWidget(statusLabel);
    createGraphs();
    createNumbers(status[1], status[2]);

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

QList<QString> ComplianceDashboardItem::determineComplianceStatus(
    const QVector<double>& measurements,
    double safeThreshold,
    const QVector<bool>& isComply
    ) {
    int exceedanceCount = std::count_if(measurements.begin(), measurements.end(),
                                        [safeThreshold](double val) { return val > safeThreshold; }); //compare with threshold
    int statusCount = std::count_if(isComply.begin(), isComply.end(),
                                    [](bool val) { return val == true; }); // compare with compliance data
    double average = std::round((std::accumulate(measurements.begin(), measurements.end(), 0.0)/measurements.size())*100.0)/100.0;
    double exceedancePercentage = (double)exceedanceCount / measurements.size() * 100;
    double statusPercentage = std::round(((double)statusCount/isComply.size() * 100)*100.0)/100.0;
    if (exceedancePercentage <= 10 && statusPercentage >= 50) return {"Compliant", QString::number(statusPercentage),QString::number(average)};
    if (exceedancePercentage <= 25 && statusPercentage >= 50) return {"Caution", QString::number(statusPercentage),QString::number(average)};
    return {"Non-compliant", QString::number(statusPercentage),QString::number(average)};
}


QColor ComplianceDashboardItem::getComplianceColor(const QString& status) {
    if (status == "Compliant") return QColor(0, 180, 0);     // green
    if (status == "Caution") return QColor(255, 165, 0);     // yellow
    return QColor(255, 0, 0);                               // red
}

void ComplianceDashboardItem::createGraphs() {

    QTableWidget *table = new QTableWidget(itemLocation.size() + 1, 4);
    table->setHorizontalHeaderLabels({"Location", "Measurement", "Threshold", "Compliance"});

    // Header row formatting
    QFont headerFont = table->horizontalHeaderItem(0)->font();
    headerFont.setBold(true);
    for (int col = 0; col < 4; ++col) {
        table->horizontalHeaderItem(col)->setFont(headerFont);
    }

    // Populate table with location data
    for (int i = 0; i < itemLocation.size(); i++) {
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
    table->setMinimumSize(QSize(600, 300));
    graphLayout->addWidget(table);
    itemLayout->addLayout(graphLayout);
}

void ComplianceDashboardItem::createNumbers(QString percentage, QString average){
    numbers = new QLabel();
    numbers->setText("Compliance ratio: \n" + percentage + '%' + "\n\nAverage amount \nof pollutant:\n " + average + "%");
    numbers->setMinimumWidth(50);
    numbers->setAlignment(Qt::AlignCenter);
    numbers->setStyleSheet("font-size: 18px; font-weight: bold;");
    itemLayout->addWidget(numbers);
}
/*
search?
sorting
location and pollutant filter
pop up
loading time
*/