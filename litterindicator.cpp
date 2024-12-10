#include "litterindicator.hpp"

LitterIndicator::LitterIndicator(SampleModel* sharedModel, QWidget *parent)
    : QWidget(parent), model(sharedModel) {
    std::cout << "Model address: " << model << std::endl;

    barChart = new QChart();
    chartView = new QChartView(barChart);

    createWidgets();
    makeConnections();
    addChart();
}

void LitterIndicator::createWidgets() {
    filterLayout = new QHBoxLayout();
    siteBox = new QComboBox();
    materialBox = new QComboBox();
    searchButton = new QPushButton("Search");

    siteBox->addItems(siteNames);
    materialBox->addItems(waterTypes);

    filterLayout->addWidget(siteBox);
    filterLayout->addWidget(materialBox);
    filterLayout->addWidget(searchButton);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(filterLayout);
    setLayout(mainLayout);
}

void LitterIndicator::addChart() {
    chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(chartView);
}

void LitterIndicator::changeChart(const QString& site, const QString& waterType) {
    barChart->removeAllSeries();  
    QList<QAbstractAxis*> axes = barChart->axes();  
    for (QAbstractAxis* axis : axes) {
        barChart->removeAxis(axis);  
    }

    QBarSeries* series = new QBarSeries(); 

  
    QMap<QString, double> waterTypeData;

    for (int i = 0; i < model->rowCount(QModelIndex()); i++) {
        QString rowSite = model->data(model->index(i, 1), Qt::DisplayRole).toString(); 
        QString rowWaterType = model->data(model->index(i, 2), Qt::DisplayRole).toString();  
        double litterCount = model->data(model->index(i, 4), Qt::DisplayRole).toDouble();  

        if ((site == "All locations" || rowSite == site) && (waterType == "All types" || rowWaterType == waterType)) {
            waterTypeData[rowWaterType] += litterCount;  
        }
    }

    qDebug() << "Water type data: " << waterTypeData;

    
    QStringList categories;  
    for (const QString& water : waterTypeData.keys()) {
        categories << water;
    }


    qDebug() << "Categories for X axis: " << categories;

    QBarSet* barSet = new QBarSet("Litter Count");
    for (const QString& water : categories) {
        *barSet << waterTypeData.value(water, 0); 
    }

    series->append(barSet);  

    barChart->addSeries(series);


    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    barChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("Litter Count");
    axisY->setRange(0, 100); 
    barChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    barChart->setTitle(QString("Litter Levels by Water Type (%1)").arg(waterType));

    chartView->setChart(barChart);
}

void LitterIndicator::search() {
    QString selectedSite = siteBox->currentText();
    QString selectedWaterType = materialBox->currentText();
    changeChart(selectedSite, selectedWaterType); 
}

void LitterIndicator::makeConnections() {
    connect(searchButton, SIGNAL(clicked()), this, SLOT(search()));
}
