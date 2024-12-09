
#include <QtWidgets>
#include <QtCharts>
#include <QEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QMovie>
#include "model.hpp"

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(QTabWidget* tabWidget, SampleModel* sharedModel, QWidget* parent = nullptr);

private:
    void createCards();
    void setupLayout();

    QVBoxLayout* mainLayout;
    QGridLayout* cardLayout;
    QLabel* titleLabel;
    QPushButton* pollutantOverviewCard;
    QPushButton* litterIndicatorCard;
    QPushButton* fluorinatedCompoundsCard;
    QPushButton* complianceDashboardCard;
    QComboBox* timeFilter;
    SampleModel* model;
    double complianceTrueCount=0;
    double complianceFalseCount=0;

    QTabWidget* mainTabWidget; 
    QChartView* createSummaryChart(const QString& title, const QVector<double>& data, const QStringList& categories);


};

// Add this as a separate class in your header file
class ChartClickHandler : public QObject {
public:
    ChartClickHandler(QTabWidget* tabWidget, int tabIndex) 
        : m_tabWidget(tabWidget), m_tabIndex(tabIndex) {}

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                m_tabWidget->setCurrentIndex(m_tabIndex);
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QTabWidget* m_tabWidget;
    int m_tabIndex;
};

