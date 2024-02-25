#include "ValveCentralHole.h"


void ValveCentralHole::InitializeUIElements() {
	// Initialize Tab Widget and add the necessary tabs
	tab_widget.reset(ui.tab_widget);
	menu_bar_.reset(ui.options_menu);
	calibrate_tab_ = std::make_unique<CalibrateWidget>(this);
	measure_tab_ = std::make_unique<MeasureWidget>(this);

	options_menu_.reset(menu_bar_->addMenu("Options"));
	options_actions_.push_back(options_menu_->addAction("Helper Gauge Diameter Measure Tool"));

	// Removing the starting tabs from the QTabWidget
	tab_widget->clear();

	tab_widget->addTab(calibrate_tab_.get(), "Calibrate");
	tab_widget->addTab(measure_tab_.get(), "Measure");

	// Setting the window icon
	setWindowTitle("Valve Central Hole Calculator");
	setWindowIcon(QIcon("window_icon.jpg"));
}

void ValveCentralHole::ConnectEventListeners() {
	connect(tab_widget.get(), &QTabWidget::tabBarClicked, this, [this](int tab_index) {
		qDebug() << "Switched to tab with index: " << tab_index;
		tab_widget->setCurrentIndex(tab_index);
		if (tab_index == 1)
		{
			measure_tab_->RefreshCalibrationFactor();
		}
		});

	connect(options_menu_.get(), &QMenu::triggered, this, [this](QAction* triggered)
		{
			qDebug() << "Activated action: " << triggered->text();
		});
}


ValveCentralHole::ValveCentralHole(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	InitializeUIElements();
	ConnectEventListeners();
}


ValveCentralHole::~ValveCentralHole()
{}
