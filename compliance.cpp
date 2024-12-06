#include "compliance.hpp"
#include "complianceItem.hpp"
#include "model.hpp"  // Include your model header
#include <QtCharts>
#include <QAbstractItemModel>

ComplianceDashboard::ComplianceDashboard(SampleModel* model, QWidget *parent)
    : QWidget(parent), m_model(model) {
    createWidgets();
    extractComplianceData();
    createFilterControls();
}
void ComplianceDashboard::createFilterControls()
{
    QWidget* filterWidget = new QWidget(this);
    QHBoxLayout* filterLayout = new QHBoxLayout(filterWidget);

    // Location filter
    QLabel* locationLabel = new QLabel("Location:", this);
    m_locationSearch = new QLineEdit(this);  // Use QLineEdit for search
    m_locationSearch->setPlaceholderText("Search locations...");
    m_locationFilter = new QComboBox(this);
    m_locationFilter->addItem("All Locations");
    m_locationFilter->setEditable(false);  // Keep it read-only for user interaction

    // Pollutant filter
    QLabel* pollutantLabel = new QLabel("Pollutant:", this);
    m_pollutantSearch = new QLineEdit(this);  // Use QLineEdit for search
    m_pollutantSearch->setPlaceholderText("Search pollutants...");
    m_pollutantFilter = new QComboBox(this);
    m_pollutantFilter->addItem("All Pollutants");
    m_pollutantFilter->setEditable(false);

    // Compliance filter
    QLabel* complianceLabel = new QLabel("Compliance:", this);
    m_complianceFilter = new QComboBox(this);
    m_complianceFilter->addItems({"All", "Compliant", "Non-Compliant"});

    // Filter and Reset buttons
    QPushButton* applyButton = new QPushButton("Apply Filters", this);
    QPushButton* resetButton = new QPushButton("Reset", this);

    filterLayout->addWidget(locationLabel);
    filterLayout->addWidget(m_locationSearch);  // Add the search bar
    filterLayout->addWidget(m_locationFilter);
    filterLayout->addWidget(pollutantLabel);
    filterLayout->addWidget(m_pollutantSearch);  // Add the search bar
    filterLayout->addWidget(m_pollutantFilter);
    filterLayout->addWidget(complianceLabel);
    filterLayout->addWidget(m_complianceFilter);
    filterLayout->addWidget(applyButton);
    filterLayout->addWidget(resetButton);

    m_mainLayout->insertWidget(1, filterWidget);

    // Connect search functionality
    connect(m_locationSearch, &QLineEdit::textChanged, this, &ComplianceDashboard::filterLocations);
    connect(m_pollutantSearch, &QLineEdit::textChanged, this, &ComplianceDashboard::filterPollutants);

    connect(applyButton, &QPushButton::clicked, this, &ComplianceDashboard::applyFilters);
    connect(resetButton, &QPushButton::clicked, this, &ComplianceDashboard::resetFilters);
}

void ComplianceDashboard::filterLocations(const QString& text)
{
    m_locationFilter->clear();
    m_locationFilter->addItem("All Locations");

    for (const QString& location : m_locations) {
        if (location.contains(text, Qt::CaseInsensitive)) {
            m_locationFilter->addItem(location);
        }
    }
}

void ComplianceDashboard::filterPollutants(const QString& text)
{
    m_pollutantFilter->clear();
    m_pollutantFilter->addItem("All Pollutants");

    for (const QString& pollutant : m_pollutants) {
        if (pollutant.contains(text, Qt::CaseInsensitive)) {
            m_pollutantFilter->addItem(pollutant);
        }
    }
}

void ComplianceDashboard::resetFilters()
{
    m_locationSearch->clear();
    m_pollutantSearch->clear();
    m_locationFilter->setCurrentText("All Locations");
    m_pollutantFilter->setCurrentText("All Pollutants");
    m_complianceFilter->setCurrentText("All");

    for (auto& data : m_complianceItems) {
        data.item->setVisible(true);
    }
}


void ComplianceDashboard::createWidgets() {
    m_mainLayout = new QVBoxLayout(this);
    QLabel* titleLabel = new QLabel("Pollutant Compliance Dashboard", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    m_mainLayout->addWidget(titleLabel);

    m_scrollArea = new QScrollArea(this);
    m_scrollContent = new QWidget();
    m_scrollLayout = new QVBoxLayout(m_scrollContent);
    m_scrollArea->setWidget(m_scrollContent);
    m_scrollArea->setWidgetResizable(true);

    m_mainLayout->addWidget(m_scrollArea);
    setLayout(m_mainLayout);
}

void ComplianceDashboard::extractComplianceData() {
    // Group data by parameter (pollutant)
    QMap<QString, QVector<QString>> pollutantLocations;
    QMap<QString, QVector<double>> pollutantMeasurements;
    QMap<QString, double> pollutantThresholds;
    QMap<QString, QVector<bool>> pollutantCompliance;

    // Assuming your model has a method to get number of rows
    int rowCount = m_model->rowCount(QModelIndex());;

    for (int row = 0; row < rowCount; ++row) {
        // Retrieve data from model using appropriate methods
        QString site = m_model->data(m_model->index(row, 1), Qt::DisplayRole).toString();
        QString parameter = m_model->data(m_model->index(row, 2), Qt::DisplayRole).toString();
        double result = m_model->data(m_model->index(row, 3), Qt::DisplayRole).toDouble();
        QString unit = m_model->data(m_model->index(row, 4), Qt::DisplayRole).toString();
        bool compliance = m_model->data(m_model->index(row, 6), Qt::DisplayRole).toString().toLower() == "true";
        // Store unique locations and pollutants for filters
        m_locations.insert(site);
        m_pollutants.insert(parameter);
        // You might want to parse the compliance string to get a threshold
        // This is a placeholder - adjust based on your actual compliance data format
        double threshold = 10.0;  // Default threshold, modify as needed
        pollutantLocations[parameter].append(site);
        if(unit=="mg/l"){
            pollutantMeasurements[parameter].append(result*1000);            
        }else{
            pollutantMeasurements[parameter].append(result);     
        }
        pollutantCompliance[parameter].append(compliance);

        // Only set threshold once per pollutant
        if (!pollutantThresholds.contains(parameter)) {
            pollutantThresholds[parameter] = threshold;
        }
    }
    /*
    qDebug() << "Locations:";
    for (const QString& location : m_locations) {
        qDebug() << location;
    }

    qDebug() << "Pollutants:";
    for (const QString& pollutant : m_pollutants) {
        qDebug() << pollutant;
    }
*/
    //m_locationFilter->addItems(m_locations.values());
    //m_pollutantFilter->addItems(m_pollutants.values());

    // Add data to dashboard
    for (auto it = pollutantMeasurements.begin(); it != pollutantMeasurements.end(); ++it) {
        QString pollutantName = it.key();
        QVector<double> measurements = it.value();
        QVector<QString> locations = pollutantLocations.value(pollutantName);
        double safeThreshold = pollutantThresholds.value(pollutantName, 10.0);
        QVector<bool> compliances = pollutantCompliance.value(pollutantName);
        addPollutantComplianceData(locations, pollutantName, safeThreshold, measurements, compliances);
    }
}

void ComplianceDashboard::addPollutantComplianceData(
    const QVector<QString>& locations,
    const QString& pollutantName,
    double safeThreshold,
    const QVector<double>& measurements,
    const QVector<bool>& isCompliance
    ) {
    // If item already exists, update it
    if (m_complianceItems.contains(pollutantName)) {
        m_complianceItems[pollutantName].item->updateData(locations, safeThreshold, measurements, isCompliance);
        return;
    }

    // Create new compliance item
    ComplianceDashboardItem* item = new ComplianceDashboardItem(
        locations,
        pollutantName,
        safeThreshold,
        measurements,
        isCompliance,
        this
        );

    m_scrollLayout->addWidget(item);
    m_complianceItems[pollutantName].item = item;
    m_complianceItems[pollutantName].isCompliant = item->ComplianceStatus;
}


void ComplianceDashboard::applyFilters()
{
    QString selectedCompliance = m_complianceFilter->currentText();

    for (auto it = m_complianceItems.begin(); it != m_complianceItems.end(); ++it) {
        QString key = it.key();
        ComplianceData& data = it.value();
        bool show = true;

        // Apply compliance filter
        if (selectedCompliance != "All") {
            bool shouldBeCompliant = (selectedCompliance == "Compliant");
            if (data.isCompliant != shouldBeCompliant) {
                show = false;
            }
        }

        data.item->setVisible(show);
    }
}



