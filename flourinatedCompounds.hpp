
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
class QScatterSeries;
class QChart;
class QChartView;
class QDateTimeAxis;
class QValueAxis;
class QDateTime;
class QAreaSeries;

class QTimer;

class FlourinatedCompounds: public QWidget
{
  Q_OBJECT

  public:
    FlourinatedCompounds(SampleModel* sharedModel);
    void retranslateUi();

  private:
    void createWidgets();
    void makeConnections();
    void createXAxis();
    void changeChart(QString site, QString compound);
    void createColoredRegions();

    QLineSeries* series;
    QScatterSeries* scatterSeries;
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
    QString unit;

    double padding;
    double third;
    double miny = 100;
    double maxy = 0;

    QStringList siteNames = {
      tr("AIRE AT BEAL BRIDGE"),
      tr("DON AT A638 NORTH BRIDGE RD - DONCASTER"),
      tr("ESK AT RUSWARP - BANKSIDE"),
      tr("WHARFE ABOVE TADCASTER WEIR")
    };
    QStringList compoundNames = {
      tr("pFoctanoate"), 
      tr("PFOS"), 
      tr("PFHxS-B"), 
      tr("8:2 FTSA"), 
      tr("PFDoS"), 
      tr("PFHxSA"), 
      tr("6:2 FTSA"), 
      tr("PFDS"), 
      tr("PFHpS"), 
      tr("PFBS"), 
      tr("HFPO-DA"), 
      tr("PFUnDS")
    };

  private slots:
    void search();
    void showPointTooltip(const QPointF& point, bool state);

};