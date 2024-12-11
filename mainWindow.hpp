
#include <QMainWindow>
#include <QtWidgets>

class QTabWidget;
class QWidget;
class QLabel;
class QComboBox;
class QStackedWidget;
class QToolBar;
class QHBoxLayout;

class SampleModel;
class TableWidget;
class Dashboard;
class FlourinatedCompounds;
class PollutantOverview;
class ComplianceDashboard;
class LitterIndicator;

class QTranslator;

class MainDashboard: public QMainWindow
{
  Q_OBJECT

  public:
    MainDashboard();

  private:
    void createWidgets();
    void createTabBar();
    void createPages();
    void makeConnections();
    void makeShortcuts();
    void retranslateUi();

    QTranslator appTranslator;

    QTabWidget* tabBar;
    QWidget* header;
    QComboBox* langChoice;
    QStackedWidget* mainWidget;
    QLabel* shortcutReminder;
    QVBoxLayout* verticalLayout;
    SampleModel* sharedModel;

    TableWidget* tableWidget;
    Dashboard* dashboard;
    FlourinatedCompounds* flourinatedCompounds;
    PollutantOverview* pollutantOverview;
    ComplianceDashboard* complianceDashboard;
    LitterIndicator* litterIndicator;
    QToolBar* topToolBar;
    QPushButton* home;

    QHBoxLayout* headerLayout;

    int index;

  private slots:
    void changeLanguage(const QString&);
    
};