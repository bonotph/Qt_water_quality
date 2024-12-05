#include <QtWidgets>
#include "tableWindow.hpp"

TableWidget::TableWidget(SampleModel* sharedModel)
{
  model = sharedModel;

  createWidgets();
  createLayouts();
}

void TableWidget::createWidgets()
{
  mainLayout = new QVBoxLayout();

  tableOverView = new QTableView();
  tableOverView->setModel(model);

  tableOverView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void TableWidget::createLayouts()
{
  mainLayout->addWidget(tableOverView, 1);

  setLayout(mainLayout);

  setMinimumSize(800, 600);
}

