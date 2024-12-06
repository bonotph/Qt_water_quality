
#include <QtWidgets>

// Forward declarations
class ComplianceDashboardItem;
class SampleModel;

class ComplianceDashboard : public QWidget {
    Q_OBJECT
public:
    explicit ComplianceDashboard(SampleModel* model, QWidget *parent = nullptr);

    void extractComplianceData();
    void addPollutantComplianceData(
        const QVector<QString>& locations,
        const QString& pollutantName,
        double safeThreshold,
        const QVector<double>& measurements,
        const QVector<bool>& isCompliance
    );

private:
    void createWidgets();
    void updateComplianceStatus();
    void createFilterControls();
    void applyFilters();
    void resetFilters();

    // Model
    SampleModel* m_model;

    // Layouts and widgets
    QVBoxLayout* m_mainLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_scrollContent;
    QVBoxLayout* m_scrollLayout;

    // Data storage
    struct ComplianceData {
        ComplianceDashboardItem* item;
        QString location;
        bool isCompliant;
    };

    QMap<QString, ComplianceData> m_complianceItems;

    // Filters
    QComboBox* m_locationFilter;
    QComboBox* m_pollutantFilter;
    QComboBox* m_complianceFilter;
    QLineEdit* m_locationSearch;  // Added for location search
    QLineEdit* m_pollutantSearch; // Added for pollutant search

    // Data sets
    QSet<QString> m_locations;
    QSet<QString> m_pollutants;

private slots:
    void filterLocations(const QString& text);
    void filterPollutants(const QString& text);
};

