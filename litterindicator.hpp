#include <QtWidgets>
#include <QStringList>

#include "model.hpp"

class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QPushButton;
class QChart;
class QChartView;
class QString;

class QBarSeries;
class QBarCategoryAxis;
class QValueAxis;

class LitterIndicator : public QWidget
{
    Q_OBJECT

    public:
        LitterIndicator(SampleModel* sharedModel);
        void retranslateUi();

    private:
        void createWidgets();
        void makeConnections();
        void changeChart(const QString &site, const QString &waterType);

        SampleModel *model;

        QVBoxLayout *mainLayout;
        QHBoxLayout *filterLayout;
        QComboBox *siteBox;
        QComboBox *materialBox;
        QComboBox *waterTypeBox;
        QPushButton *searchButton;
        QChart *barChart;
        QChartView *chartView;

        QString selectedSite;
        QString selectedWaterType;

        QBarSeries* series;
        QBarCategoryAxis* axisX;
        QValueAxis* axisY;

        QStringList siteNames = {
            "All locations",
            "MALHAM TRAN",
            "DON AT A638 NORTH BRIDGE RD - DONCASTER",
            "ESK AT RUSWARP - BANKSIDE",
            "WHARFE ABOVE TADCASTER WEIR"};

        QStringList waterTypes = {
            "All types",
            "Lake",
            "Pond",
            "Reservoir",
            "River",
            "Stream"};

    private slots:
        void search();
};
