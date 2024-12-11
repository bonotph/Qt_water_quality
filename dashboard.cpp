#include "dashboard.hpp"

Dashboard::Dashboard(QTabWidget* tabWidget, SampleModel* sharedModel, QWidget* parent) : QWidget(parent), model(sharedModel), mainTabWidget(tabWidget)
{
    mainLayout = new QVBoxLayout(this);
    createCards();
    setupLayout();
}

void Dashboard::createCards()
{
    titleLabel = new QLabel(tr("Dashboard Overview"));
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin-bottom: 10px;");
    timeFilter = new QComboBox();
    timeFilter->addItems({tr("last day"),tr("last month"),tr("last year")});
    

    pollutantOverviewCard = new QPushButton(tr("Pollutant Overview"));
    litterIndicatorCard = new QPushButton(tr("Environmental Litter"));
    fluorinatedCompoundsCard = new QPushButton(tr("Fluorinated Compounds"));
    complianceDashboardCard = new QPushButton(tr("Compliance Dashboard"));

    const QString cardStyle = R"(
        QPushButton {
            border: 1px solid #ddd;
            border-radius: 8px;
            background-color: #f9f9f9;
            font-size: 18px;
            padding: 10px;
        }
        QPushButton:hover {
            background-color: #e6e6e6;
        }
    )";

    pollutantOverviewCard->setStyleSheet(cardStyle);
    litterIndicatorCard->setStyleSheet(cardStyle);
    fluorinatedCompoundsCard->setStyleSheet(cardStyle);
    complianceDashboardCard->setStyleSheet(cardStyle);

    // Direct connections to switch tabs
    connect(pollutantOverviewCard, &QPushButton::clicked, this, [this]() {
        mainTabWidget->setCurrentIndex(1); 
    });
    connect(litterIndicatorCard, &QPushButton::clicked, this, [this]() {
        mainTabWidget->setCurrentIndex(3); 
    });
    connect(fluorinatedCompoundsCard, &QPushButton::clicked, this, [this]() {
        mainTabWidget->setCurrentIndex(4); 
    });
    connect(complianceDashboardCard, &QPushButton::clicked, this, [this]() {
        mainTabWidget->setCurrentIndex(5); 
    });
}


void Dashboard::setupLayout()
{
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(timeFilter);
    cardLayout = new QGridLayout();

    int rownum = model->rowCount(QModelIndex());
    // data, modify
    for (int i = 0; i < rownum; ++i) {
        QString site = model->data(model->index(i, 1), Qt::DisplayRole).toString();
        QString parameter = model->data(model->index(i, 2), Qt::DisplayRole).toString();
        double result = model->data(model->index(i, 3), Qt::DisplayRole).toDouble();
        QString unit = model->data(model->index(i, 4), Qt::DisplayRole).toString();
        bool compliance = model->data(model->index(i, 6), Qt::DisplayRole).toString().toLower() == "true";

        if (compliance) {
            complianceTrueCount++;
        } else {
            complianceFalseCount++;
        }
    }

    // summary charts
    QChartView* pollutantChart = createSummaryChart(
        tr("Pollutant Trends"),
        {30, 50, 70, 60},
        {tr("Jan"), tr("Feb"), tr("Mar"), tr("Apr")}
    );
    QChartView* litterChart = createSummaryChart(
        tr("Litter Levels"),
        {15, 25, 35, 40},
        {tr("Beach"), tr("Lake"), tr("River"), tr("Sea")}
    );
    QChartView* fluorinatedChart = createSummaryChart(
        tr("Fluorinated Compounds"),
        {1.2, 2.8, 1.9, 3.4},
        {tr("Site A"), tr("Site B"), tr("Site C"), tr("Site D")}
    );
    QChartView* complianceChart = createSummaryChart(
        tr("Compliance Status"),
        {complianceTrueCount, complianceFalseCount},
        {tr("Compliant"), tr("Non-Compliant")}
    );

    // add charts and layouts
    if (pollutantChart) cardLayout->addWidget(pollutantChart, 0, 0);
    if (litterChart) cardLayout->addWidget(litterChart, 0, 1);
    if (fluorinatedChart) cardLayout->addWidget(fluorinatedChart, 1, 0);
    if (complianceChart) cardLayout->addWidget(complianceChart, 1, 1);
    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(cardLayout);
}

void Dashboard::retranslateUi()
{
    titleLabel->setText(tr("Dashboard Overview"));
    pollutantOverviewCard->setText(tr("Pollutant Overview"));
    litterIndicatorCard->setText(tr("Environmental Litter"));
    fluorinatedCompoundsCard->setText(tr("Fluorinated Compounds"));
    complianceDashboardCard->setText(tr("Compliance Dashboard"));
    timeFilter->clear();
    timeFilter->addItems({tr("last day"),tr("last month"),tr("last year")});
}

QChartView* Dashboard::createSummaryChart(const QString& title, const QVector<double>& data, const QStringList& categories){   
    //check empty
    if (data.isEmpty() || categories.isEmpty()) {
        qDebug() << "Empty data or categories passed to createSummaryChart";
        return nullptr;
    }

    // create the chart
    QChart* chart = new QChart();
    chart->setTitle(title);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    if (title == "Compliance Status") {
        QHorizontalStackedBarSeries* series = new QHorizontalStackedBarSeries();
        
        QList<qreal> dummyCount;
        QStringList complianceNames = {tr("Compliant"), tr("Non-Compliant")};
        QList<QColor> colors = {QColor(0, 128, 0), QColor(255, 0, 0)}; // Green, Red

        for(int i = 0; i < data.size(); ++i) {
            auto barSet = new QBarSet(complianceNames.at(i));
            barSet->setColor(colors[i]);

            // Clear previous dummy values and add new ones
            if(i > 0) dummyCount.replace(i-1, 0);
            dummyCount << data[i];
            
            barSet->append(dummyCount);
            series->append(barSet);
        }

        series->setLabelsVisible(true);
        series->setLabelsFormat(QStringLiteral("@value"));
        series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);

        chart->addSeries(series);

        // Configure Y-axis (categories)
        QBarCategoryAxis* axisY = new QBarCategoryAxis();
        axisY->append(categories);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Configure X-axis (values)
        QValueAxis* axisX = new QValueAxis();
        double maxVal = *std::max_element(data.begin(), data.end());
        axisX->setRange(0, maxVal * 1.2);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);
        axisX->applyNiceNumbers();
    }
    else {
        // other charts, modify as needed
        QBarSeries* series = new QBarSeries();
        QBarSet* set = new QBarSet(title);
        
        for (double value : data)
            *set << value;

        series->append(set);
        chart->addSeries(series);

        // x axis
        QBarCategoryAxis* axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        // y-axis
        QValueAxis* axisY = new QValueAxis();
        axisY->setRange(0, *std::max_element(data.begin(), data.end()) * 1.2);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
    }

    // chart view
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMouseTracking(true);

    // tab navigating
    int tabIndex = -1;
    if (title == "Pollutant Trends") {
        tabIndex = 1;
    } else if (title == "Litter Levels") {
        tabIndex = 3;
    } else if (title == "Fluorinated Compounds") {
        tabIndex = 4;
    } else if (title == "Compliance Status") {
        tabIndex = 5;
    }

    // add click event
    if (tabIndex != -1) {
        chartView->installEventFilter(new ChartClickHandler(mainTabWidget, tabIndex));
    }

    return chartView;
}