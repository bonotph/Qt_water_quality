#include "compliance.hpp"
#include "complianceItem.hpp"
#include "model.hpp" 
#include <QDebug>

ComplianceDashboard::ComplianceDashboard(SampleModel* model, QWidget *parent)
    : QWidget(parent), dataModel(model) {
    if (!model) {
        qDebug() << "Error: Null model";
        return;
    }
    createWidgets();
    getData();
    createFilter();
}

void ComplianceDashboard::createFilter(){
    if (!mainLayout) {
        qDebug() << "Error: No main layout";
        return;
    }

    //fliter widget
    QWidget* filterWidget = new QWidget(this);
    QHBoxLayout* filterLayout = new QHBoxLayout(filterWidget);

    // compliance filter combo box
    complianceFilter = new QComboBox(this);
    complianceFilter->addItems({"All", "Compliant", "Non-Compliant"});
    complianceFilter->setMinimumWidth(300); 
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // apply and reset buttons
    QPushButton* applyButton = new QPushButton("Apply", this);
    QPushButton* resetButton = new QPushButton("Reset", this);
    applyButton->setFixedWidth(80);  // Set a fixed, shorter width
    resetButton->setFixedWidth(80);  // Set a fixed, shorter width

    // add to layout
    filterLayout->addWidget(complianceFilter);
    filterLayout->addWidget(applyButton);
    filterLayout->addWidget(resetButton);
    mainLayout->insertWidget(1, filterWidget);

    // connect signals and slots
    connect(applyButton, &QPushButton::clicked, this, &ComplianceDashboard::applyFilter);
    connect(resetButton, &QPushButton::clicked, this, &ComplianceDashboard::resetFilter);
}

void ComplianceDashboard::createWidgets(){
/*     if (!mainLayout) {
        qDebug() << "Error: No main layout";
        return;
    } */

    //basic layout
    mainLayout = new QVBoxLayout(this);
    QLabel* title = new QLabel("Pollutant Compliance Dashboard", this);
    title->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    scrollContent = new QWidget(this);
    scrollLayout = new QVBoxLayout(scrollContent);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(title);
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

void ComplianceDashboard::getData(){
    if (!dataModel) {
        qDebug() << "Error: Null data model";
        return;
    }

    QMap<QString, QVector<QString>> locations;
    QMap<QString, QVector<double>> values;
    QMap<QString, double> thresholds;
    QMap<QString, QVector<bool>> compliances;
    
    int rownum = dataModel->rowCount(QModelIndex());

    // clear data
    locations.clear();
    values.clear();
    thresholds.clear();
    compliances.clear();
    items.clear();

    //extract data
    for(int i = 0; i < rownum; ++i){
        QString site = dataModel->data(dataModel->index(i, 1), Qt::DisplayRole).toString();
        QString parameter = dataModel->data(dataModel->index(i, 2), Qt::DisplayRole).toString();
        double result = dataModel->data(dataModel->index(i, 3), Qt::DisplayRole).toDouble();
        QString unit = dataModel->data(dataModel->index(i, 4), Qt::DisplayRole).toString();
        bool compliance = dataModel->data(dataModel->index(i, 6), Qt::DisplayRole).toString().toLower() == "true";
        
        double threshold = 10.0;  
        
        // handle unit conversion and store data
        if(unit == "mg/l"){
            values[parameter].append(result * 1000);            
        } else {
            values[parameter].append(result);     
        }
        locations[parameter].append(site);
        compliances[parameter].append(compliance);
        if (!thresholds.contains(parameter)) {
            thresholds[parameter] = threshold;
        }
    }

    // add data
    for(auto it = values.begin(); it != values.end(); ++it){
        const QString& temp_pollutants = it.key();
        const QVector<QString>& temp_locations = locations[temp_pollutants];
        double temp_threshold = thresholds[temp_pollutants];
        const QVector<double>& temp_values = it.value();
        const QVector<bool>& temp_compliances = compliances[temp_pollutants];
        
        addData(temp_locations, temp_pollutants, temp_threshold, temp_values, temp_compliances);
    }
}


void ComplianceDashboard::addData(
    const QVector<QString>& locations,
    const QString& pollutants,
    double thresholds,
    const QVector<double>& values, 
    const QVector<bool>& compliances
){
    if(items.contains(pollutants)){
        items[pollutants].item->updateData(locations, thresholds, values, compliances);
        return;
    }

    ComplianceDashboardItem* item = new ComplianceDashboardItem(
        locations,
        pollutants,
        thresholds,
        values,
        compliances,
        this
    );
    scrollLayout->addWidget(item);
    items[pollutants].item=item;
    items[pollutants].isCompliant = item->complianceStatus;
}


void ComplianceDashboard::applyFilter(){
QString selectedCompliance = complianceFilter->currentText();

for (auto it = items.begin(); it != items.end(); ++it) {
    QString key = it.key();
    ComplianceData& data = it.value();
    bool show = true;

    if (selectedCompliance != "All") {
        bool shouldBeCompliant = (selectedCompliance == "Compliant");
        if (data.isCompliant != shouldBeCompliant) {
            show = false;
        }
    }
    data.item->setVisible(show);
}
}


void ComplianceDashboard::resetFilter(){
complianceFilter->setCurrentText("All");
for (auto& data : items) {
    data.item->setVisible(true);
}
}