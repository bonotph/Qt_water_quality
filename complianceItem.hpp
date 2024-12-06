
#include <QLabel>
#include <QtCharts>

class ComplianceDashboardItem : public QWidget {
    Q_OBJECT

public:
    ComplianceDashboardItem(
        const QVector<QString>& locations,
        const QString& pollutantName,
        double safeThreshold,
        const QVector<double>& measurements,
        const QVector<bool>& isCompliance,
        QWidget *parent = nullptr
        );

    void updateData(const QVector<QString>& locations,double safeThreshold, const QVector<double>& measurements,const QVector<bool>& complianceStatus);
    bool ComplianceStatus;

private:
    void createMainWidget();
    QChartView* createScrollableLineChart();
    QChartView* createGroupedBarChart();
    QTableWidget* createSummaryTable();
    void createComplianceVisualization();
    QString determineComplianceStatus(const QVector<double>& measurements, double safeThreshold, const QVector<bool>& isComply);
    QColor getComplianceColor(const QString& status);

    QVector<QString> m_location;
    QString m_pollutantName;
    double m_safeThreshold;
    QVector<double> m_measurements;
    QVector<bool> m_isComply;

    QVBoxLayout* m_layout;
    QLabel* m_nameLabel;
    QLabel* m_statusLabel;
    QChartView* m_chartView;
};
