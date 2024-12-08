#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QGraphicsView>
#include <QString>
#include "model.hpp"

class LitterIndicator : public QMainWindow
{
    Q_OBJECT

public:
    explicit LitterIndicator(SampleModel* sharedModel, QWidget *parent = nullptr);
    ~LitterIndicator();

private slots:
    void onSearchClicked();

private:
    SampleModel* model;  
    QGraphicsView *graphicsView;
    QComboBox *locationComboBox;
    QComboBox *datePickerComboBox;
    QPushButton *searchButton;

    QString currentLanguage;

    void updateChart(const QString &location, const QString &dateRange);
};
