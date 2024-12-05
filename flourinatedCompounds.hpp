
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

class FlourinatedCompounds: public QWidget
{
  Q_OBJECT

  public:
    FlourinatedCompounds(SampleModel* sharedModel);

  private:
    void createWidgets();
    void createLayouts();
    void makeConnections();

    void changeChart(QString site, QString compound);

    void addChart();
    QLineSeries* series;
    QChart* chart;
    QChartView* chartView;
    QDateTimeAxis* xAxis;

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
        "Fluoroxypyr",
        "TrCFMethan",
        "PFOS",
        "pFoctanoate",
        "PFDoS",
        "PFoctncAcid",
        "4:2 FTSA",
        "PFDS",
        "PFnonncAcid",
        "ADONA",
        /*"PFdecncAcid",
        "5:3 FTCA",
        "FBSA",
        "PFundencAcid",
        "MeFOSAA-L",
        "PFUnDS",
        "PFdodencAcid",
        "PFBS",
        "EtFOSAA-L",
        "PFtetdncAcid",
        "PFecHS",
        "PFTrDA",
        "11Cl-PF3OUdS",
        "N-MeFOSA",
        "PFPeS",
        "PFBA",
        "EtFOSA",
        "PFheptncAcid",
        "FOSA",
        "PFpentncAcid",
        "PFMOBA",
        "6:2 FTSA",
        "PFHxSA",
        "MeFOSAA-B",
        "PFNS",
        "PFEESA",
        "PFOS (B)",
        "3:3 FTCA",
        "NFDHA",
        "8:2 FTSA",
        "PFHpS",
        "PFHxS-L",
        "HFPO-DA",
        "EtFOSAA-B",
        "PFODA",*/
        "PFhexncAcid",
        "7:3 FTCA",
        "PFMOPrA",
        "PFOS (L)",
        "9Cl-PF3ONS",
        "PFHxS-B"
    };

  private slots:
    void search();

};