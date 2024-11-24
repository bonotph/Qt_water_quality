
#include <QtWidgets>
#include "model.hpp"

class SampleModel;
class QVBoxLayout;
class QTableView;

class TableWidget: public QWidget
{
  Q_OBJECT

  public:
    TableWidget();

  private:
    void createWidgets();
    void createLayouts();

    QVBoxLayout* mainLayout;
    QTableView* tableOverView;
    SampleModel* model;
   
};