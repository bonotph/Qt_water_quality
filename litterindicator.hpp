#ifndef LITTERINDICATOR_HPP
#define LITTERINDICATOR_HPP

#include <QtWidgets>
#include <QtCharts>
#include "model.hpp"

class LitterIndicator : public QWidget {
    Q_OBJECT

public:
    explicit LitterIndicator(SampleModel* sharedModel, QWidget *parent = nullptr);

private slots:
    //void updateChart();  
    void addChart();    
    void search();
    void changeChart(const QString& site, const QString& waterType); 

private:
    SampleModel* model;  

    QVBoxLayout* mainLayout;
    QHBoxLayout* filterLayout;
    QComboBox* siteBox;
    QComboBox* materialBox;
    QComboBox* waterTypeBox;     
    QPushButton* searchButton;

    QStringList siteNames = {
        "All locations",
        "MALHAM TRAN",
        "DON AT A638 NORTH BRIDGE RD - DONCASTER",
        "ESK AT RUSWARP - BANKSIDE",
        "WHARFE ABOVE TADCASTER WEIR"
    };
    QStringList waterTypes = { 
        "All types",
        "Lake",
        "Pond",
        "Reservoir",
        "River",
        "Stream"
    };

    // 图表相关
    void createWidgets();  
    void makeConnections();  
    QChart* barChart;        
    QChartView* chartView;   
};

#endif // LITTERINDICATOR_HPP
