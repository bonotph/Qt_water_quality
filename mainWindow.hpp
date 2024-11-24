
#include <QMainWindow>

class QTabWidget;
class QWidget;
class QLabel;
class QComboBox;
class QStackedWidget;

class QHBoxLayout;

class TableWidget;
class FlourinatedCompounds;

class MainDashboard: public QMainWindow
{
  Q_OBJECT

  public:
    MainDashboard();

  private:
    void createWidgets();
    void createTabBar();
    void createPages();
    void createActions();

    QTabWidget* tabBar;
    QWidget* header;
    QLabel* langLabel;
    QComboBox* langChoice;
    QStackedWidget* mainWidget;

    TableWidget* tableWidget;
    FlourinatedCompounds* flourinatedCompounds;

    QHBoxLayout* headerLayout;
};