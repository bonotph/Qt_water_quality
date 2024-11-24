
#include <QtWidgets>
#include "flourinatedCompounds.hpp"

FlourinatedCompounds::FlourinatedCompounds()
{
  createWidgets();
  
}

void FlourinatedCompounds::createWidgets()
{
  filterLayout = new QHBoxLayout();
  siteBox = new QComboBox();
  compoundBox = new QComboBox();
  searchButton = new QPushButton("Search");

  siteBox->addItem("All sites");
  siteBox->addItem("Site 1");
  siteBox->addItem("Site 2");

  compoundBox->addItem("All compounds");
  compoundBox->addItem("PFAS");
  compoundBox->addItem("Compound 2");
  
  filterLayout->addWidget(siteBox);
  filterLayout->addWidget(compoundBox);
  filterLayout->addWidget(searchButton);

  mainLayout = new QVBoxLayout();
  mainLayout->addLayout(filterLayout);

  setLayout(mainLayout);
}