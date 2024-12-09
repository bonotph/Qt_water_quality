
#include <QtWidgets>
#include "mainWindow.hpp"
#include "pollutantOverview.hpp"
#include "tableWindow.hpp"
#include "flourinatedCompounds.hpp"
#include "compliance.hpp"
#include "litterindicator.hpp"
#include "dashboard.hpp"

MainDashboard::MainDashboard():QMainWindow()
{

  sharedModel = new SampleModel();
  sharedModel->updateFromFile("../Y-2024.csv");
  
  createWidgets();
  createTabBar();
  createPages();
  createActions();

  setMinimumSize(1000, 800);
  setWindowTitle(tr("Water Quality Monitor"));

}

void MainDashboard::createWidgets()
{
  home = new QPushButton(tr("Home"));
  home->setFixedWidth(80);
  header = new QWidget();
  tabBar = new QTabWidget();
  //langLabel = new QLabel("Language:");
  langChoice = new QComboBox();
  mainWidget = new QStackedWidget();
  tableWidget = new TableWidget(sharedModel);
  dashboard = new Dashboard(tabBar, sharedModel);
  flourinatedCompounds = new FlourinatedCompounds(sharedModel);
  pollutantOverview = new PollutantOverview(sharedModel);
  complianceDashboard = new ComplianceDashboard(sharedModel);
  litterIndicator = new LitterIndicator(sharedModel);
  setCentralWidget(mainWidget);
}
void MainDashboard::createTabBar()
{
    topToolBar = new QToolBar(this);
    topToolBar->setMovable(false);
    topToolBar->setFloatable(false);
    topToolBar->addWidget(home);
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    topToolBar->addWidget(spacer);
    langChoice->addItems({tr("English"), tr("French"), tr("German"), tr("Spanish"), tr("Italian")});
    topToolBar->addWidget(langChoice);

    tabBar->addTab(new QWidget(), tr("Dashboard"));
    tabBar->addTab(new QWidget(), tr("Pollutant Overview"));
    tabBar->addTab(new QWidget(), tr("Persistant Organic Pollutants"));
    tabBar->addTab(new QWidget(), tr("Environmental Litter"));
    tabBar->addTab(new QWidget(), tr("Flourinated Compounds"));
    tabBar->addTab(new QWidget(), tr("Compliance Dashboard"));
    tabBar->addTab(new QWidget(), tr("Table"));

    // Create shortcut reminder label
    shortcutReminder = new QLabel(tr("Keyboard shortcuts: Press 1-7 to switch tabs"));
    shortcutReminder->setAlignment(Qt::AlignCenter);
    shortcutReminder->setStyleSheet("QLabel { color: #666666; font-size: 11px; padding: 2px; }");

    // Create a vertical layout to stack the tab bar and reminder
    QVBoxLayout* verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(tabBar);
    verticalLayout->addWidget(shortcutReminder);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    // Use the vertical layout in the header
    headerLayout = new QHBoxLayout();
    QWidget* tabContainer = new QWidget();
    tabContainer->setLayout(verticalLayout);
    headerLayout->addWidget(tabContainer);

    setMenuWidget(header);
    header->setLayout(headerLayout);
}

void MainDashboard::createPages()
{
  addToolBar(Qt::TopToolBarArea, topToolBar);
  //exchange for real pages later
  mainWidget->addWidget(dashboard);  
  mainWidget->addWidget(pollutantOverview);
  mainWidget->addWidget(new QWidget());
  mainWidget->addWidget(litterIndicator);
  mainWidget->addWidget(flourinatedCompounds);
  mainWidget->addWidget(complianceDashboard);

  mainWidget->addWidget(tableWidget);

  // default page that is shown in beginning
  mainWidget->setCurrentIndex(0);
  tabBar->setCurrentIndex(0);

}


void MainDashboard::createActions()
{
    // Existing connections
    connect(tabBar, SIGNAL(currentChanged(int)), mainWidget, SLOT(setCurrentIndex(int)));
    connect(home, &QPushButton::clicked, this, [this]() {
        tabBar->setCurrentIndex(0);  
        mainWidget->setCurrentIndex(0);  
    });
   QShortcut* shortcutDashboard = new QShortcut(QKeySequence("1"), this);
    connect(shortcutDashboard, &QShortcut::activated, this, [this]() {
        tabBar->setCurrentIndex(0);  // Dashboard
        mainWidget->setCurrentIndex(0);
    });

    QShortcut* shortcutPollutantOverview = new QShortcut(QKeySequence("2"), this);
    connect(shortcutPollutantOverview, &QShortcut::activated, this, [this]() {
        tabBar->setCurrentIndex(1);  // Pollutant Overview
        mainWidget->setCurrentIndex(1);
    });

    QShortcut* shortcutPersistantOrganicPollutants = new QShortcut(QKeySequence("3"), this);
    connect(shortcutPersistantOrganicPollutants, &QShortcut::activated, this, [this]() {
        tabBar->setCurrentIndex(2);  // Persistant Organic Pollutants
        mainWidget->setCurrentIndex(2);
    });

    QShortcut* shortcutEnvironmentalLitter = new QShortcut(QKeySequence("4"), this);
    connect(shortcutEnvironmentalLitter, &QShortcut::activated, this, [this]() {
        tabBar->setCurrentIndex(3);  // Environmental Litter
        mainWidget->setCurrentIndex(3);
    });

    QShortcut* shortcutFlourinatedCompounds = new QShortcut(QKeySequence("5"), this);
    connect(shortcutFlourinatedCompounds, &QShortcut::activated, this, [this]() {
        tabBar->setCurrentIndex(4);  // Flourinated Compounds
        mainWidget->setCurrentIndex(4);
    });

    QShortcut* shortcutComplianceDashboard = new QShortcut(QKeySequence("6"), this);
    connect(shortcutComplianceDashboard, &QShortcut::activated, this, [this]() {
        tabBar->setCurrentIndex(5);  // Compliance Dashboard
        mainWidget->setCurrentIndex(5);
    });

    QShortcut* shortcutTable = new QShortcut(QKeySequence("7"), this);
    connect(shortcutTable, &QShortcut::activated, this, [this]() {
        tabBar->setCurrentIndex(6);  // Table
        mainWidget->setCurrentIndex(6);
    });

}





