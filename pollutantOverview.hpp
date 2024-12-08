#pragma once

#include <QtWidgets>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include "model.hpp"

class PollutantOverview : public QWidget
{
    Q_OBJECT

public:
    // Constructor with SampleModel reference
    PollutantOverview(SampleModel* sharedModel, QWidget* parent = nullptr);

private:
    SampleModel* model;           // Store the model reference
    QComboBox* pollutantSearchBox; // Combo box for pollutant search
    QPushButton* searchButton;     // Button to search
    QLineSeries* series;           // Data series for the graph
    QChart* chart;                 // Chart object for displaying trends
    QChartView* chartView;         // Chart view to render the chart
    QVBoxLayout* mainLayout;       // Main layout for the widget
    QHBoxLayout* searchLayout;     // Layout for search box and button

    // Helper function to create widgets
    void createWidgets();

    // Helper function to create layouts
    void createLayouts();

    // Helper function to make connections between widgets
    void makeConnections();

    // Slot to handle search button click
    void onSearchClicked();

    // Function to update the graph based on selected pollutant
    void updateGraph(const QString& pollutantName);
};