#include "litterindicator.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QSpacerItem>

LitterIndicator::LitterIndicator(SampleModel* sharedModel, QWidget *parent)
    : model(sharedModel), QMainWindow(parent)
{
    setWindowTitle("Environmental Litter Indicator");

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout: vertical
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // ====== Time, location, and search button layout ======
    QHBoxLayout *selectLayout = new QHBoxLayout();
    
    // Location selection
    locationComboBox = new QComboBox(this);
    locationComboBox->addItem("All locations");
    locationComboBox->addItem("Location 1");
    locationComboBox->addItem("Location 2");
    selectLayout->addWidget(locationComboBox);

    // Time selection
    datePickerComboBox = new QComboBox(this);
    datePickerComboBox->addItem("All time");
    datePickerComboBox->addItem("Last Week");
    datePickerComboBox->addItem("Last Month");
    datePickerComboBox->addItem("Last Year");
    selectLayout->addWidget(datePickerComboBox);

    // Search button
    searchButton = new QPushButton("Search", this);
    connect(searchButton, &QPushButton::clicked, this, &LitterIndicator::onSearchClicked);
    selectLayout->addWidget(searchButton);

    mainLayout->addLayout(selectLayout);

    // Graphics view
    graphicsView = new QGraphicsView(this);
    graphicsView->setStyleSheet("background-color: white; border: 5px solid gray;");
    graphicsView->setMinimumSize(600, 400);
    mainLayout->addWidget(graphicsView);

    // Connect signals for debug logging
    connect(locationComboBox, &QComboBox::currentTextChanged, this, [](const QString &text) {
        qDebug() << "Selected Location:" << text;
    });

    connect(datePickerComboBox, &QComboBox::currentTextChanged, this, [](const QString &text) {
        qDebug() << "Selected Date Range:" << text;
    });




    //data:
    for (int i = 0; i < model->rowCount(QModelIndex()); ++i)
    {
        QString pollutant = model->data(model->index(i, 2), Qt::DisplayRole).toString();  // get column stuff
    }












}

LitterIndicator::~LitterIndicator()
{
    // Destructor for cleanup if needed
}


void LitterIndicator::onSearchClicked()
{
    QString selectedLocation = locationComboBox->currentText();
    QString selectedDate = datePickerComboBox->currentText();

    qDebug() << "Search clicked with Location:" << selectedLocation << ", Date:" << selectedDate;
    updateChart(selectedLocation, selectedDate);
}


void LitterIndicator::updateChart(const QString &location, const QString &dateRange)
{
    qDebug() << "Updating chart for Location:" << location << ", Date:" << dateRange;
}