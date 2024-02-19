#include "MeasureWidget.h"
#include "messageboxhelper.h"


static void CreateBinaryImagePreview(MeasureWidget& measure_widget, const Mat& mat)
{
	Mat temp = mat.clone();
	BinarizeImageHelper::BinarizeImage(temp, measure_widget.GetThresholdValue(), ThresholdMode::INVERTED);
	measure_widget.SetPreviewMat(temp);
}



MeasureWidget::MeasureWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::MeasureWidget)
{
	ui->setupUi(this);

	calibration_factor_label_.reset(ui->current_calibration_factor);
	calculated_area_label_.reset(ui->valve_area_label);
	select_valve_image_.reset(ui->select_valve_image);
	preview_btn_.reset(ui->preview_btn);
	measure_btn_.reset(ui->measure_btn);
	original_image_.reset(ui->original_valve);
	binarized_image_.reset(ui->binarized_valve);
	threshold_value_input_.reset(ui->valve_threshold_input);

	file_dialog_ = std::make_unique<QFileDialog>(this, "Select Valve Image");

	InitializeUIElements();
	ConnectEventListeners();
}


void MeasureWidget::InitializeUIElements() {
	calibration_factor_label_->setText(calibration_factor_label_->text() + QString::number(CalibrateWidget::GetCalibrationFactor()));

	threshold_value_input_->setMinimum(0);
	threshold_value_input_->setMaximum(255);
	threshold_value_input_->setValue(127);
}

int MeasureWidget::GetThresholdValue() const
{
	return threshold_value_;
}

void MeasureWidget::SetPreviewMat(Mat preview_mat)
{
	binarized_image_preview_mat_ = preview_mat;
}


void MeasureWidget::ConnectEventListeners() {
	connect(select_valve_image_.get(), &QPushButton::clicked, this, [this]() {
		file_dialog_->show();
		});
	connect(preview_btn_.get(), &QPushButton::clicked, this, [this]() {
		// TODO Implement functionality for previewing the binary image here
		if (current_image_mat_.empty() || binarized_image_preview_mat_.empty())
		{
			MessageBoxHelper::ShowErrorDialog("Select an input image first");
			return;
		}

		QImage preview_image(binarized_image_preview_mat_.data, binarized_image_preview_mat_.cols, binarized_image_preview_mat_.rows, QImage::Format_Grayscale8);
		if (preview_image.isNull())
		{
			MessageBoxHelper::ShowErrorDialog("An error occurred while trying to display the preview image");
			return;
		}

		QPixmap preview_pixmap = QPixmap::fromImage(preview_image);
		if (preview_pixmap.isNull())
		{
			MessageBoxHelper::ShowErrorDialog("An error occurred while trying to display the preview image");
			return;
		}

		binarized_image_->setPixmap(preview_pixmap.scaled(QSize(IMAGE_WIDTH, IMAGE_HEIGHT)));
		});
	connect(measure_btn_.get(), &QPushButton::clicked, this, [this]() {
		// TODO Implement functionality for measuring the valve here
		if (current_image_mat_.empty() || binarized_image_preview_mat_.empty())
		{
			MessageBoxHelper::ShowErrorDialog("Select an input image first");
			return;
		}

		auto thread_handle = std::async(std::launch::async, [this]()
			{
				std::pair<unsigned long long, unsigned long long> num_on_off_pixels = BinarizeImageHelper::GetNumberOfOnAndOffPixels(binarized_image_preview_mat_);
				emit this->onAreaCalculationComplete(BinarizeImageHelper::CalculateValveArea(num_on_off_pixels.first, CalibrateWidget::GetCalibrationFactor()));
			});
		});
	connect(this, &MeasureWidget::onAreaCalculationComplete, this, [this](double valve_area)
		{
			QString unit_suffix = GetUnitSuffix(CalibrateWidget::current_unit_selection_);
			calculated_area_label_->setText("Calculated Valve Area: " + QString::number(valve_area) + unit_suffix);
		});
	connect(file_dialog_.get(), &QFileDialog::fileSelected, this, [this](const QString& filename) {
		qDebug() << "Valve Image filepath: " << filename;
		if (!filename.contains(".jpg", Qt::CaseInsensitive) && !filename.contains(".png", Qt::CaseInsensitive)) {
			MessageBoxHelper::ShowErrorDialog("Selected file is not a valid image format (.jpg or .png)");
			return;
		}
		current_image_mat_ = imread(filename.toStdString(), IMREAD_GRAYSCALE);
		if (current_image_mat_.empty())
		{
			MessageBoxHelper::ShowErrorDialog("An error occurred while trying to open the image file");
			return;
		}

		QPixmap image = QPixmap(filename).scaled(QSize(IMAGE_WIDTH, IMAGE_HEIGHT));
		original_image_->setPixmap(image);

		auto thread_handle = std::async(std::launch::async, &CreateBinaryImagePreview, std::ref(*this), std::ref(current_image_mat_));
		});
	connect(threshold_value_input_.get(), &QSpinBox::valueChanged, this, [this](int new_value) {
		threshold_value_ = new_value;
		auto thread_handle = std::async(std::launch::async, &CreateBinaryImagePreview, std::ref(*this), std::ref(current_image_mat_));
		});
}

void MeasureWidget::RefreshCalibrationFactor()
{
	QString unit_suffix = GetUnitSuffix(CalibrateWidget::current_unit_selection_);
	calibration_factor_label_->setText("Calibration Factor: " + QString::number(CalibrateWidget::GetCalibrationFactor()) + " " + unit_suffix);
}

MeasureWidget::~MeasureWidget()
{
	delete ui;
}