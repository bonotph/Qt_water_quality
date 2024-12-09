
#include <QMainWindow>
#include <QtWidgets>

class QTabWidget;
class QWidget;
class QLabel;
class QComboBox;
class QStackedWidget;

class QHBoxLayout;

class SampleModel;
class TableWidget;
class Dashboard;
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
    void setupUndoRedoActions();
    void undoLastAction();
    void redoLastAction();
    void updateUndoRedoActions();
    void setupShortcuts();
    void openSettingsPage();

    QTabWidget* tabBar;
    QWidget* header;
    QLabel* langLabel;
    QComboBox* langChoice;
    QStackedWidget* mainWidget;
    QLabel* shortcutReminder;

    SampleModel* sharedModel;

    TableWidget* tableWidget;
    Dashboard* dashboard;
    FlourinatedCompounds* flourinatedCompounds;
    PollutantOverview* pollutantOverview;
    ComplianceDashboard* complianceDashboard;
    LitterIndicator* litterIndicator;
    QAction* undoAction;
    QAction* redoAction;   
    QToolBar* topToolBar;

    QHBoxLayout* headerLayout;
};