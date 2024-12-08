
#include <QMainWindow>

class QTabWidget;
class QWidget;
class QLabel;
class QComboBox;
class QStackedWidget;

class QHBoxLayout;

class SampleModel;
class TableWidget;
class FlourinatedCompounds;
class PollutantOverview;
class ComplianceDashboard;
class LitterIndicator;

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

    SampleModel* sharedModel;

    TableWidget* tableWidget;
    FlourinatedCompounds* flourinatedCompounds;
    PollutantOverview* pollutantOverview;
    ComplianceDashboard* complianceDashboard;
    LitterIndicator* litterIndicator;


    QHBoxLayout* headerLayout;
};