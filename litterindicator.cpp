#include <QtWidgets>
#include <QtCharts>
#include <QList>

#include "litterindicator.hpp"

LitterIndicator::LitterIndicator(SampleModel* sharedModel)
{
    model = sharedModel;

    createWidgets();
    makeConnections();
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

    barChart = new QChart();
    chartView = new QChartView(barChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(chartView);
    setLayout(mainLayout);
}

void LitterIndicator::changeChart(const QString& site, const QString& waterType) {
    barChart->removeAllSeries(); 

    QList<QAbstractAxis*> axes = barChart->axes();
    for (QAbstractAxis* axis : axes) {
        barChart->removeAxis(axis);
    }

    series = new QBarSeries(); 

    QMap<QString, double> waterTypeData;

    for (int i = 0; i < model->rowCount(QModelIndex()); i++) {
        QString rowSite = model->data(model->index(i, 1), Qt::DisplayRole).toString(); 
        QString rowWaterType = model->data(model->index(i, 7), Qt::DisplayRole).toString();  
        double litterCount = model->data(model->index(i, 3), Qt::DisplayRole).toDouble(); 

        if ((site == "All locations" || rowSite == site) && (waterType == "All types" || rowWaterType.contains(waterType, Qt::CaseInsensitive))) {
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

    axisX = new QBarCategoryAxis();
    axisX->append(categories);
    barChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setTitleText("Litter Count");
    axisY->setRange(0, 100); 
    barChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    barChart->setTitle(QString("Litter Levels by Water Type (%1)").arg(waterType));

    chartView->setChart(barChart);
}

void LitterIndicator::search() {
    selectedSite = siteBox->currentText();
    selectedWaterType = materialBox->currentText();
    qDebug() << "Selected site: " << selectedSite << ", selected water type: " << selectedWaterType;
    changeChart(selectedSite, selectedWaterType); 
}

void LitterIndicator::retranslateUi() {
    // add widgets for translation
}

void LitterIndicator::makeConnections() {
    connect(searchButton, SIGNAL(clicked()), this, SLOT(search()));
}
