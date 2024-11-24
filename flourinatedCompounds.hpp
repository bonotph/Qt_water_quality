
#include <QtWidgets>
#include "model.hpp"

class QComboBox;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

class FlourinatedCompounds: public QWidget
{
  Q_OBJECT

  public:
    FlourinatedCompounds();

  private:
    void createWidgets();
    void createLayouts();

    QHBoxLayout* filterLayout;
    QVBoxLayout* mainLayout;
    QComboBox* siteBox;
    QComboBox* compoundBox;
    QPushButton* searchButton;

};