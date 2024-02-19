#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QFileDialog>
#include <memory>
#include "measurewidget.h"
#include "ui_ValveCentralHole.h"

class ValveCentralHole : public QMainWindow
{
    Q_OBJECT

public:
    ValveCentralHole(QWidget* parent = nullptr);
    ~ValveCentralHole();

private:
    Ui::ValveCentralHoleClass ui;
    std::unique_ptr<QTabWidget> tab_widget;
    std::unique_ptr<CalibrateWidget> calibrate_tab_;
    std::unique_ptr<MeasureWidget> measure_tab_;

    void InitializeUIElements();
    void ConnectEventListeners();
};
