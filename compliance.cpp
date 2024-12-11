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

void ComplianceDashboard::createWidgets(){
/*     if (!mainLayout) {
        qDebug() << "Error: No main layout";
        return;
    } */

    //basic layout
    mainLayout = new QVBoxLayout(this);
    QLabel* title = new QLabel(tr("Pollutant Compliance Dashboard"), this);
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
    QMap<QString, QVector<bool>> compliances;
    
    int rownum = dataModel->rowCount(QModelIndex());

    // clear data
    locations.clear();
    values.clear();
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
    }

    // add data
    for(auto it = values.begin(); it != values.end(); ++it){
        const QString& temp_pollutants = it.key();
        const QVector<QString>& temp_locations = locations[temp_pollutants];
        const QVector<double>& temp_values = it.value();
        const QVector<bool>& temp_compliances = compliances[temp_pollutants];
        
        addData(temp_locations, temp_pollutants, temp_values, temp_compliances);
    }
}


void ComplianceDashboard::createFilter() {
    if (!mainLayout) {
        qDebug() << "Error: No main layout";
        return;
    }

    // filter widget
    QWidget* filterWidget = new QWidget(this);
    QHBoxLayout* filterLayout = new QHBoxLayout(filterWidget);

    // search input
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText(tr("Search pollutants or sites..."));
    searchLineEdit->setMinimumWidth(200);

    // compliance filter combo box
    complianceFilter = new QComboBox(this);
    complianceFilter->addItems({tr("All"), tr("Compliant"), tr("Non-Compliant")});
    complianceFilter->setMinimumWidth(150);

    //sort(dummy)
    sortComboBox = new QComboBox(this);
    sortComboBox->addItems({tr("No Sort(dummy)"), tr("Average (Low to High)"), tr("Average (High to Low)")});
    sortComboBox->setMinimumWidth(150);

    // apply and reset buttons
    QPushButton* resetButton = new QPushButton(tr("Reset"), this);
    resetButton->setFixedWidth(80);
    locationFilter = new QComboBox(this);
    locationFilter->addItems({tr("All Locations (dummy)"), tr("Yorkshire")});

    // add to layout
    filterLayout->addWidget(new QLabel(tr("Search:")));
    filterLayout->addWidget(searchLineEdit);
    filterLayout->addWidget(new QLabel(tr("Compliance:")));
    filterLayout->addWidget(complianceFilter);
    filterLayout->addWidget(locationFilter);
    filterLayout->addWidget(sortComboBox);
    filterLayout->addStretch();
    filterLayout->addWidget(resetButton);
    mainLayout->insertWidget(1, filterWidget);

    // connect signals and slots
    connect(searchLineEdit, &QLineEdit::textChanged, this, &ComplianceDashboard::applyFilter);
    connect(complianceFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ComplianceDashboard::applyFilter);
    connect(resetButton, &QPushButton::clicked, this, &ComplianceDashboard::resetFilter);
}


void ComplianceDashboard::addData(
    const QVector<QString>& locations,
    const QString& pollutants,
    const QVector<double>& values,
    const QVector<bool>& compliances
) {
    if(items.contains(pollutants)) {
        items[pollutants].item->updateData(locations, values, compliances);
        return;
    }

    ComplianceDashboardItem* item = new ComplianceDashboardItem(
        locations,
        pollutants,
        values,
        compliances,
        this
    );
    scrollLayout->addWidget(item);
    items[pollutants].item = item;
    items[pollutants].isCompliant = item->complianceStatus;
    items[pollutants].averageValue = item->averageValue;
}


void ComplianceDashboard::applyFilter() {
    QString searchText = searchLineEdit->text().toLower();
    QString selectedCompliance = complianceFilter->currentText();

    for (auto it = items.begin(); it != items.end(); ++it) {
        QString key = it.key();
        ComplianceData& data = it.value();
        bool show = true;

        // Search filter
        if (!searchText.isEmpty()) {
            bool matchFound = false;
            
            // Check if pollutant name matches
            if (key.toLower().contains(searchText)) {
                matchFound = true;
            }
            
            // Check if any location matches
            for (const QString& location : data.item->getLocations()) {
                if (location.toLower().contains(searchText)) {
                    matchFound = true;
                    break;
                }
            }

            if (!matchFound) {
                show = false;
            }
        }

        // Compliance filter
        if (show && selectedCompliance != "All") {
            bool shouldBeCompliant = (selectedCompliance == "Compliant");
            if (data.isCompliant != shouldBeCompliant) {
                show = false;
            }
        }

        data.item->setVisible(show);
    }
}

void ComplianceDashboard::resetFilter() {
    // Reset search line edit
    searchLineEdit->clear();
    
    // Reset compliance filter
    complianceFilter->setCurrentText("All");

    // Show all items
    for (auto& data : items) {
        data.item->setVisible(true);
    }
}

void ComplianceDashboard::retranslateUi() {
    //add widgets
}