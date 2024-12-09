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
    QComboBox* sortComboBox;
    void sortItems(const QString& criteria);
    double calculateAverage(const QVector<double>& values);

    SampleModel* dataModel;
    
    QVBoxLayout* mainLayout;
    QScrollArea* scrollArea;
    QVBoxLayout* scrollLayout;
    QWidget* scrollContent;
    QLineEdit* searchLineEdit;

    struct ComplianceData {
        ComplianceDashboardItem* item;
        QString location;
        bool isCompliant;
        double averageValue; 
    };

    QMap<QString, ComplianceData> items;

    QComboBox* complianceFilter;
    QComboBox* locationFilter;

};