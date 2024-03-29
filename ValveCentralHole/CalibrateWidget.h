#pragma once

#include "BinarizeImageHelper.h"
#include "ui_CalibrateWidget.h"
#include <QObject>
#include <QComboBox>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <future>
#include <memory>


constexpr int IMAGE_WIDTH = 930;
constexpr int IMAGE_HEIGHT = 510;

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui {
	class CalibrateWidget;
}

QT_END_NAMESPACE

enum class CurrentUnitSelection
{
	MILLIMETERS,
	INCHES
};


QString GetUnitSuffix(CurrentUnitSelection current_unit_selection);

class CalibrateWidget : public QWidget
{
	Q_OBJECT
public:
	CalibrateWidget(QWidget* parent = nullptr);
	static double GetGaugeDiameter();
	int GetThresholdValue() const;
	static double GetCalibrationFactor();
	void SetPreviewMat(Mat preview_mat);

	static CurrentUnitSelection current_unit_selection_;

signals:
	void OnCalibrationComplete(double calibration_factor);

private:
	Ui::CalibrateWidget* ui;
	Mat current_image_mat_;
	Mat binarized_preview_image_mat_;

	static double gauge_diameter_;
	int threshold_value_ = 127;
	static double calibration_factor_;
	QString selected_image_filename_;

	std::unique_ptr<QDoubleSpinBox> diameter_input_;
	std::unique_ptr<QComboBox> diameter_unit_selection_;
	std::unique_ptr<QSpinBox> threshold_input_;
	std::unique_ptr<QFileDialog> file_select_;
	std::unique_ptr<QPushButton> select_file_button_;
	std::unique_ptr<QVBoxLayout> widget_layout_;
	std::unique_ptr<QLabel> original_image_;
	std::unique_ptr<QLabel> binarized_image_;
	std::unique_ptr<QPushButton> preview_btn_;
	std::unique_ptr<QPushButton> calibrate_btn_;
	std::unique_ptr<QLabel> calibration_factor_label_;

	void InitializeUIElements();
	void ConnectEventListeners();
};

