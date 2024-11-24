#include <QtWidgets>
#include "tableWindow.hpp"

TableWidget::TableWidget()
{
  createWidgets();
  createLayouts();
}

void TableWidget::createWidgets()
{
  mainLayout = new QVBoxLayout();

  SampleModel* model = new SampleModel();
  tableOverView = new QTableView();
  tableOverView->setModel(model);
  model->updateFromFile("../Y-2024.csv");

  tableOverView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void TableWidget::createLayouts()
{
  mainLayout->addWidget(tableOverView, 1);

  setLayout(mainLayout);

  setMinimumSize(800, 600);
}

