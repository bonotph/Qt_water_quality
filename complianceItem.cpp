#include "complianceItem.hpp"
#include <QtCharts>
ComplianceDashboardItem::ComplianceDashboardItem(
    const QVector<QString>& locations,
    const QString& pollutants,
    const QVector<double>& values,
    const QVector<bool>& compliances,
    QWidget *parent
    ) : QWidget(parent),
    itemLocation(locations),
    itemPollutant(pollutants),
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

    // Determine compliance status first
    QList<QString> status = determineComplianceStatus(itemValue, itemIsComply);
    averageValue = status[2].toDouble();
    complianceStatus = (status[0] == "Compliant" || status[0] == "Caution");

    // Only add details button if NOT compliant
    if (!complianceStatus) {
        QPushButton* detailsButton = new QPushButton(tr("&View Non-Compliance Details"), this);
        detailsButton->setStyleSheet("background-color: #FF4444; color: white; font-weight: bold;");
        connect(detailsButton, &QPushButton::clicked, this, &ComplianceDashboardItem::showNonComplianceDetails);
        graphLayout->addWidget(detailsButton);
    }

    // compliance status
    statusLabel = new QLabel(this);
    statusLabel->setText(QString(tr("Compliance Status: %1")).arg(status[0]));
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
    const QVector<double>& values,
    const QVector<bool>& compliances
    ) {
    itemLocation = locations;
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
    const QVector<bool>& isComply
    ) {
    int statusCount = std::count_if(isComply.begin(), isComply.end(),
                                    [](bool val) { return val == true; }); // compare with compliance data
    double average = std::round((std::accumulate(measurements.begin(), measurements.end(), 0.0)/measurements.size())*100.0)/100.0;
    double statusPercentage = std::round(((double)statusCount/isComply.size() * 100)*100.0)/100.0;
    if (statusPercentage >= 50) return {tr("Compliant"), QString::number(statusPercentage),QString::number(average)};
    if (statusPercentage >= 50) return {tr("Caution"), QString::number(statusPercentage),QString::number(average)};
    return {tr("Non-compliant"), QString::number(statusPercentage),QString::number(average)};
}


QColor ComplianceDashboardItem::getComplianceColor(const QString& status) {
    if (status == "Compliant") return QColor(0, 180, 0);     // green
    if (status == "Caution") return QColor(255, 165, 0);     // yellow
    return QColor(255, 0, 0);                               // red
}

void ComplianceDashboardItem::createGraphs() {

    QTableWidget *table = new QTableWidget(itemLocation.size() + 1, 3);
    table->setHorizontalHeaderLabels({tr("Location"), tr("Measurement"), tr("Compliance")});

    // Header row formatting
    QFont headerFont = table->horizontalHeaderItem(0)->font();
    headerFont.setBold(true);
    for (int col = 0; col < 3; ++col) {
        table->horizontalHeaderItem(col)->setFont(headerFont);
    }

    // Populate table with location data
    for (int i = 0; i < itemLocation.size(); i++) {
        QTableWidgetItem *locationItem = new QTableWidgetItem(itemLocation[i]);
        QTableWidgetItem *measurementItem = new QTableWidgetItem(QString::number(itemValue[i]));
        QTableWidgetItem *complianceItem = new QTableWidgetItem(itemIsComply[i] ? tr("Compliant") : tr("Non-Compliant"));
        
        // Color code compliance
        complianceItem->setBackground(itemIsComply[i] ? QColor(200, 255, 200) : QColor(255, 200, 200));

        table->setItem(i, 0, locationItem);
        table->setItem(i, 1, measurementItem);
        table->setItem(i, 2, complianceItem);
    }

    table->resizeColumnsToContents();
    table->setMinimumSize(QSize(600, 300));
    graphLayout->addWidget(table);
    itemLayout->addLayout(graphLayout);
}

void ComplianceDashboardItem::createNumbers(QString percentage, QString average){
    numbers = new QLabel();
    numbers->setText(tr("Compliance ratio: \n") + percentage + '%' + tr("\n\nAverage amount \nof pollutant:\n ") + average + tr("ug/l"));
    numbers->setMinimumWidth(35);
    numbers->setAlignment(Qt::AlignCenter);
    numbers->setStyleSheet("font-size: 18px; font-weight: bold;");
    itemLayout->addWidget(numbers);
}
void ComplianceDashboardItem::showNonComplianceDetails() {
    QDialog* detailsDialog = new QDialog(this);
    detailsDialog->setWindowTitle(tr("Non-Compliance Details: ") + itemPollutant);
    
    QVBoxLayout* dialogLayout = new QVBoxLayout(detailsDialog);
    
    // Title and overview
    QLabel* titleLabel = new QLabel(tr("<h2>Non-Compliance Details</h2>"), detailsDialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    dialogLayout->addWidget(titleLabel);

    // Safer way to extract percentage
    QString complianceStatus = statusLabel->text();
    double percentageValue = complianceStatus.contains("%") 
        ? complianceStatus.split("%").first().split(":").last().trimmed().toDouble()
        : 0.0;

    // Compliance Statistics
    QLabel* statsLabel = new QLabel(QString(
        tr("<b>Compliance Statistics:</b><br>"
        "Average Pollutant Level: %1<br>"
        "Compliance Ratio: %2%<br>"
        "Threshold: %3")
    ).arg(QString::number(averageValue), 
          QString::number(percentageValue)));
    dialogLayout->addWidget(statsLabel);

    // Get specific causes
    QStringList causes = getNonComplianceCauses();
    for (const QString& cause : causes) {
        QLabel* causeItem = new QLabel("• " + cause, detailsDialog);
        dialogLayout->addWidget(causeItem);
    }

    // Detailed Location Breakdown
    QLabel* locationLabel = new QLabel(tr("<b>Most Polluted Location:</b>"), detailsDialog);
    dialogLayout->addWidget(locationLabel);
    double maxvalue;
    int locationindex;
    for (int i = 0; i < itemLocation.size(); ++i) {
        if (!itemIsComply[i] && itemValue[i]>maxvalue) {
            maxvalue = itemValue[i];
            locationindex=i;
        }
    }
    QLabel* locationItem = new QLabel(QString(tr("• %1 (Measurement: %2)"))
    .arg(itemLocation[locationindex])
    .arg(itemValue[locationindex]), detailsDialog);
    dialogLayout->addWidget(locationItem);

    // Recommended Actions
    QLabel* actionLabel = new QLabel(tr("<b>Recommended Actions:</b>"), detailsDialog);
    dialogLayout->addWidget(actionLabel);
    QStringList actions = {
        tr("Conduct detailed investigation on site"),
        tr("Implement source-addressing water treatment solutions"),
        tr("Review current measures of pollution control"),
        tr("Apply constant monitoring and evaluations")
    };
    for (const QString& action : actions) {
        QLabel* actionItem = new QLabel("• " + action, detailsDialog);
        dialogLayout->addWidget(actionItem);
    }

    // Close button
    QPushButton* closeButton = new QPushButton(tr("&Close"), detailsDialog);
    connect(closeButton, &QPushButton::clicked, detailsDialog, &QDialog::accept);
    dialogLayout->addWidget(closeButton);

    detailsDialog->setLayout(dialogLayout);
    detailsDialog->setMinimumWidth(400);
    detailsDialog->exec();
}

QStringList ComplianceDashboardItem::getNonComplianceCauses() {
    // map pollutants to causes
    QMap<QString, QStringList> pollutantCauses = {
        {tr("Nitrates"), {
            tr("Excessive fertilizer use"),
            tr("Agricultural runoff"),
            tr("Sewage overflow"),
            tr("Inadequate water treatment")
        }},
        {"Lead", {
            tr("Old plumbing infrastructure"),
            tr("Industrial emissions"),
            tr("Soil contamination"),
            tr("Historical industrial activities")
        }},
        {"Mercury", {
            tr("Industrial process emissions"),
            tr("Coal-fired power plant discharges"),
            tr("Improper waste disposal"),
            tr("Natural geological sources")
        }},
        {"Arsenic", {
            tr("Geological formations"),
            tr("Mining and industrial waste"),
            tr("Pesticide residues"),
            tr("Groundwater contamination")
        }}
    };

    // default
    QStringList genericCauses = {
        tr("Environmental contamination"),
        tr("Inadequate pollution control"),
        tr("Industrial or agricultural activities"),
        tr("Systemic infrastructure issues")
    };

    return pollutantCauses.value(itemPollutant, genericCauses);
}