#pragma once

#include "CalibrationGaugeParametersDAO.h"
#include "BinarizeImageHelper.h"
#include "ui_CalibrateWidget.h"
#include "ThreadPool.h"
#include <QObject>
#include <QComboBox>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QLabel>
#include <QSlider>
#include <QMouseEvent>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <future>
#include <memory>
#include <mutex>
#include <vector>


constexpr int IMAGE_WIDTH = 610;
constexpr int IMAGE_HEIGHT = 330;

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui {
	class CalibrateWidget;
}

QT_END_NAMESPACE



QString GetUnitSuffix(UnitSelection current_unit_selection);

class CalibrateWidget : public QWidget
{
	Q_OBJECT
public:
	CalibrateWidget(const std::unique_ptr<bool>& gauge_helper_tool_flag, QWidget* parent = nullptr);
	static double GetGaugeDiameter();
	int GetThresholdValue() const;
	static double GetCalibrationFactor();
	void SetPreviewMat(Mat preview_mat);
	const ThreadPool& GetWidgetThreadPool() const;
	void ApplyLastSavedParameters();
	void ReceiveAndDisplayCameraImage(Mat mat_from_camera);
	void ReceiveAndDisplayCameraImage(const QString& image_name);

	static UnitSelection current_unit_selection_;

signals:
	void UpdatePreviewMat();
	void OnCalibrationComplete(double calibration_factor);

protected:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
	Ui::CalibrateWidget* ui;
	const std::unique_ptr<bool>& gauge_helper_flag_;
	Mat current_image_mat_;
	Mat binarized_preview_image_mat_;

	std::mutex mutex_;
	ThreadPool tp_;

	CalibrationGaugeParameters gauge_parameters_;
	static double gauge_diameter_;
	int threshold_value_ = 127;
	static double calibration_factor_;
	QString selected_image_filename_;
	bool isCurrentlyShowingPreview = false;

	std::vector<std::pair<QPointF, QPointF>> line_points_;
	std::pair<QPointF, QPointF> current_line_point_pair_;
	bool is_on_line_start_ = true;
	QPointF start_line_;
	QPointF end_line_;

	std::unique_ptr<QDoubleSpinBox> diameter_input_;
	std::unique_ptr<QComboBox> diameter_unit_selection_;
	std::unique_ptr<QSlider> threshold_input_slider_;
	std::unique_ptr<QComboBox> threshold_mode_combo_box_;
	std::unique_ptr<QCheckBox> is_saline_checkbox_;
	std::unique_ptr<QLabel> threshold_value_label_;
	std::unique_ptr<QFileDialog> file_select_;
	std::unique_ptr<QPushButton> select_file_button_;
	std::unique_ptr<QVBoxLayout> widget_layout_;
	std::unique_ptr<QLabel> original_image_;
	std::unique_ptr<QLabel> binarized_image_;
	std::unique_ptr<QPushButton> preview_btn_;
	std::unique_ptr<QPushButton> calibrate_btn_;
	std::unique_ptr<QLabel> calibration_factor_label_;
	std::unique_ptr<QLabel> threshold_mode_tooltip_label_;
	std::unique_ptr<QLabel> saline_tooltip_label_;
	std::unique_ptr<QPushButton> clear_lines_btn_;

	void InitializeUIElements();
	void CheckForLastCalibrationParameters();
	void ConnectEventListeners();
	void DisplaySelectedImage(const QString& filename, bool should_show_binary_immediately = false);
	void DisplaySelectedImage(Mat selected_mat, bool should_show_binary_immediately = false);
	void DisplayPreviewMat();
	void DisplayCalibrationFactor();
	void SaveCurrentParametersToDatabase();
};

