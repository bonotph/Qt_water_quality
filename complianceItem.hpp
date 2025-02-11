#include <QLabel>
#include <QtCharts>

class ComplianceDashboardItem : public QWidget {
    Q_OBJECT

public:
    ComplianceDashboardItem(
        const QVector<QString>& locations,
        const QString& pollutants,
        const QVector<double>& values,
        const QVector<bool>& compliances,
        QWidget *parent = nullptr
        );

    void updateData(const QVector<QString>& locations,const QVector<double>& values,const QVector<bool>& complianceStatus);
    bool complianceStatus;
    double averageValue;
    QVector<QString> getLocations() const { return itemLocation; }

private:
    void createWidgets();
    void createGraphs();
    void createNumbers(QString percentage, QString average);
    QList<QString> determineComplianceStatus(const QVector<double>& measurements, const QVector<bool>& isComply);
    QColor getComplianceColor(const QString& status);
    QStringList getNonComplianceCauses();

    QVector<QString> itemLocation;
    QString itemPollutant;
    QVector<double> itemValue;
    QVector<bool> itemIsComply;
    
    QHBoxLayout* itemLayout;
    QVBoxLayout* graphLayout;
    QLabel* numbers;
    QLabel* itemNameLabel;
    QLabel* statusLabel;

private slots:
    void showNonComplianceDetails();

};
