
#include <QtWidgets>
#include <QStringList>
#include "model.hpp"

class QComboBox;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QString;

class SampleModel;

class QLineSeries;
class QChart;
class QChartView;
class QDateTimeAxis;
class QValueAxis;
class QDateTime;

class FlourinatedCompounds: public QWidget
{
  Q_OBJECT

  public:
    FlourinatedCompounds(SampleModel* sharedModel);

  private:
    void createWidgets();
    void makeConnections();
    void createXAxis();
    void changeChart(QString site, QString compound);

    QLineSeries* series;
    QChart* chart;
    QChartView* chartView;
    QDateTimeAxis* xAxis;
    QValueAxis* yAxis;

    QHBoxLayout* filterLayout;
    QVBoxLayout* mainLayout;
    QComboBox* siteBox;
    QComboBox* compoundBox;
    QPushButton* searchButton;

    SampleModel* model;

    QString site;
    QString compound;

    QStringList siteNames = {
      "AIRE AT BEAL BRIDGE",
      "DON AT A638 NORTH BRIDGE RD - DONCASTER",
      "ESK AT RUSWARP - BANKSIDE",
      "WHARFE ABOVE TADCASTER WEIR"
    };
    QStringList compoundNames = {
        "PFOS",
        "pFoctanoate",
        "PFHxS-B",
        "PFHxSA",
        "PFBS",
        "8:2 FTSA",
        "6:2 FTSA",
        "HFPO-DA",
        "PFDoS",
        "PFUnDS",
        "PFTrDA",
        "PFPeS",
        "PFBA",
        "PFHpS",
        "PFDS"
    };

  private slots:
    void search();

};