#include <QLabel>
#include <QtCharts>

class ComplianceDashboardItem : public QWidget {
    Q_OBJECT

public:
    ComplianceDashboardItem(
        const QVector<QString>& locations,
        const QString& pollutants,
        double thresholds,
        const QVector<double>& values,
        const QVector<bool>& compliances,
        QWidget *parent = nullptr
        );

    void updateData(const QVector<QString>& locations,double thresholds, const QVector<double>& values,const QVector<bool>& complianceStatus);
    bool complianceStatus;

private:
    void createWidgets();
    QChartView* createLineChart();
    QChartView* createGroupedBarChart();
    QTableWidget* createSummaryTable();
    void createGraphs();
    QString determineComplianceStatus(const QVector<double>& measurements, double safeThreshold, const QVector<bool>& isComply);
    QColor getComplianceColor(const QString& status);

    QVector<QString> itemLocation;
    QString itemPollutant;
    double itemThreshold;
    QVector<double> itemValue;
    QVector<bool> itemIsComply;

    QVBoxLayout* itemLayout;
    QLabel* itemNameLabel;
    QLabel* m_statusLabel;
    QChartView* m_chartView;
};

class CustomChartView : public QChartView {
public:
    explicit CustomChartView(QChart *chart, QWidget *parent = nullptr) 
        : QChartView(chart, parent) {}

protected:
    bool event(QEvent *event) override {
        if (event->type() == QEvent::ToolTip) {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
            QPoint pos = helpEvent->pos();
            
            // Convert position to chart coordinates
            QPointF scenePos = mapToScene(pos);
            QPointF chartPos = chart()->mapFromScene(scenePos);
            
            // Get X axis
            QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis*>(chart()->axes(Qt::Horizontal).first());
            if (axisX) {
                // Get the plot area rect
                QRectF plotArea = chart()->plotArea();
                // Create a rectangle for the axis area (below the plot)
                QRectF axisRect(plotArea.left(), plotArea.bottom(), plotArea.width(), 50);

                if (axisRect.contains(chartPos)) {
                    // Find which label is being hovered
                    int index = axisX->categories().size() * (chartPos.x() - plotArea.left()) / plotArea.width();
                    if (index >= 0 && index < axisX->categories().size()) {
                        QString fullText = axisX->categories().at(index);
                        QToolTip::showText(helpEvent->globalPos(), fullText);
                        return true;
                    }
                }
            }
            
            QToolTip::hideText();
        }
        return QChartView::event(event);
    }
};