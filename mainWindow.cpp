
#include <QtWidgets>
#include "mainWindow.hpp"

#include "tableWindow.hpp"
#include "flourinatedCompounds.hpp"

MainDashboard::MainDashboard():QMainWindow()
{
  sharedModel = new SampleModel();
  sharedModel->updateFromFile("../Y-2024.csv");
  
  createWidgets();
  createTabBar();
  createPages();
  createActions();

  setMinimumSize(1000, 800);
  setWindowTitle("Water Quality Monitor");
}

void MainDashboard::createWidgets()
{
  header = new QWidget();
  tabBar = new QTabWidget();
  //langLabel = new QLabel("Language:");
  langChoice = new QComboBox();
  mainWidget = new QStackedWidget();

  tableWidget = new TableWidget(sharedModel);
  flourinatedCompounds = new FlourinatedCompounds(sharedModel);

  setCentralWidget(mainWidget);
}

void MainDashboard::createTabBar()
{
  tabBar->addTab(new QWidget(), "Dashboard");
  tabBar->addTab(new QWidget(), "Pollutant Overview");
  tabBar->addTab(new QWidget(), "Persistant Organic Pollutants");
  tabBar->addTab(new QWidget(), "Environmental Litter");
  tabBar->addTab(new QWidget(), "Flourinated Compounds");
  tabBar->addTab(new QWidget(), "Compliance Dashboard");
  tabBar->addTab(new QWidget(), "Table");

  langChoice->addItems({"English", "French", "German", "Spanish", "Italian"});

  headerLayout = new QHBoxLayout();
  headerLayout->addWidget(tabBar);
  //headerLayout->addWidget(langLabel);
  headerLayout->addWidget(langChoice);

  header->setLayout(headerLayout);

  setMenuWidget(header);
}

void MainDashboard::createPages()
{
  //exchange for real pages later
  mainWidget->addWidget(new QWidget());  
  mainWidget->addWidget(new QWidget());
  mainWidget->addWidget(new QWidget());
  mainWidget->addWidget(new QWidget());
  mainWidget->addWidget(flourinatedCompounds);
  mainWidget->addWidget(new QWidget());

  mainWidget->addWidget(tableWidget);

  // default page that is shown in beginning
  mainWidget->setCurrentIndex(6);
  tabBar->setCurrentIndex(6);
}

void MainDashboard::createActions()
{
  // connect the tabBar with mainWidget to show the correct page
  connect(tabBar, SIGNAL(currentChanged(int)), mainWidget, SLOT(setCurrentIndex(int)));
}