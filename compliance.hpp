#include <QtWidgets>

class ComplianceDashboardItem;
class SampleModel;

class ComplianceDashboard: public QWidget {
    Q_OBJECT
public:
    ComplianceDashboard(SampleModel* model, QWidget *parent = nullptr);
    void getData();
    void addData(const QVector<QString>& locations,const QString& pollutants,double threshold, const QVector<double>& values, const QVector<bool>& compliances);

private:
    void createWidgets();
    void updateStatus();
    void createFilter();
    void applyFilter();
    void resetFilter();

    SampleModel* dataModel;
    
    QVBoxLayout* mainLayout;
    QScrollArea* scrollArea;
    QVBoxLayout* scrollLayout;
    QWidget* scrollContent;

    struct ComplianceData {
        ComplianceDashboardItem* item;
        QString location;
        bool isCompliant;
    };

    QMap<QString, ComplianceData> items;

    QComboBox* complianceFilter;

};