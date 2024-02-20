#include "CalibrateWidget.h"
#include "MessageBoxHelper.h"


double CalibrateWidget::gauge_diameter_ = 0;
double CalibrateWidget::calibration_factor_ = 0;

CurrentUnitSelection CalibrateWidget::current_unit_selection_ = CurrentUnitSelection::MILLIMETERS;


// Performs image binarization on a copy of the input image, and sets the preview Mat to this copy
/**
 * \brief
 * \param calibrate_widget [in]: reference to calling CalibrateWidget instance
 * \param original_mat [in] : Original, input matrix from the image file selected by the user
 * \param is_update [in] : set to true only if the function is used to update preview image as a result of the threshold slider being moved
 */
static void CreateBinaryImagePreview(CalibrateWidget& calibrate_widget, const Mat& original_mat, bool is_update) {
	Mat temp = original_mat.clone();
	BinarizeImageHelper::BinarizeImage(temp, calibrate_widget.GetThresholdValue(), ThresholdMode::INVERTED);
	calibrate_widget.SetPreviewMat(temp);

	if (is_update)
	{
		emit calibrate_widget.UpdatePreviewMat();
	}
}

QString GetUnitSuffix(CurrentUnitSelection unit_selection_)
{
	QString result;
	switch (unit_selection_)
	{
	case  CurrentUnitSelection::INCHES:
	{
		result = "in^2";
		break;
	}
	case CurrentUnitSelection::MILLIMETERS:
	{
		result = "mm^2";
		break;
	}
	}
	return result;
}


CalibrateWidget::CalibrateWidget(QWidget* parent) : QWidget(parent), ui(new Ui::CalibrateWidget) {

	ui->setupUi(this);
	InitializeUIElements();
	ConnectEventListeners();
}

void CalibrateWidget::SetPreviewMat(Mat mat)
{
	binarized_preview_image_mat_ = mat;
}

double CalibrateWidget::GetGaugeDiameter() {
	return gauge_diameter_;
}

int CalibrateWidget::GetThresholdValue() const {
	return threshold_value_;
}

double CalibrateWidget::GetCalibrationFactor() {
	return calibration_factor_;
}

void CalibrateWidget::DisplayPreviewMat()
{
	if (binarized_preview_image_mat_.empty())
	{
		MessageBoxHelper::ShowErrorDialog("Select an input image first");
		return;
	}

	QImage preview_image(binarized_preview_image_mat_.data, binarized_preview_image_mat_.cols, binarized_preview_image_mat_.rows, QImage::Format_Grayscale8);
	if (preview_image.isNull())
	{
		qDebug() << "Error converting binary image into QImage object";
		return;
	}

	QPixmap preview_pixmap = QPixmap::fromImage(preview_image);
	if (preview_pixmap.isNull())
	{
		qDebug() << "Error converting QImage into QPixmap";
		return;
	}
	binarized_image_->setPixmap(preview_pixmap.scaled(QSize(IMAGE_WIDTH, IMAGE_HEIGHT)));
}


void CalibrateWidget::InitializeUIElements()
{
	diameter_input_.reset(ui->diameter_input);
	diameter_unit_selection_.reset(ui->diameter_unit_selection);
	threshold_input_slider_.reset(ui->threshold_value_slider);
	threshold_value_label_.reset(ui->threshold_value_display);
	select_file_button_.reset(ui->select_image_file);
	original_image_.reset(ui->original_image);
	binarized_image_.reset(ui->binarized_image);
	preview_btn_.reset(ui->preview_btn);
	calibrate_btn_.reset(ui->calibrate_btn);
	calibration_factor_label_.reset(ui->calibration_factor_label);
	file_select_ = std::make_unique<QFileDialog>(this, "Select Gauge Image");

	diameter_unit_selection_->addItem("mm");
	diameter_unit_selection_->addItem("in");

	threshold_input_slider_->setMinimum(0);
	threshold_input_slider_->setMaximum(255);
	threshold_input_slider_->setSingleStep(1);
	threshold_input_slider_->setValue(127);

	diameter_input_->setMinimum(0);
	diameter_input_->setMaximum(1000000);
	diameter_input_->setDecimals(5);

	file_select_->setStyleSheet("background: white;");
}

// Add event listeners
void CalibrateWidget::ConnectEventListeners()
{
	connect(diameter_input_.get(), &QDoubleSpinBox::valueChanged, this, [this](double new_value) {
		qDebug() << "Inputted Diameter: " << new_value;
		gauge_diameter_ = new_value;
		});

	connect(diameter_unit_selection_.get(), &QComboBox::currentIndexChanged, this, [this](int index)
		{
			switch (index)
			{
				// Millimeters selection, convert from inches to millimeters
			case 0:
			{
				gauge_diameter_ *= 25.4;
				current_unit_selection_ = CurrentUnitSelection::MILLIMETERS;
				break;
			}
			// Inches selection, convert from millimeters to inches
			case 1:
			{
				gauge_diameter_ /= 25.4;
				current_unit_selection_ = CurrentUnitSelection::INCHES;
				break;
			}
			}
			diameter_input_->setValue(gauge_diameter_);
		});

	connect(threshold_input_slider_.get(), &QAbstractSlider::sliderMoved, this, [this](int value)
		{
			threshold_value_ = value;
			qDebug() << "Slider value changed: " << value;
			threshold_value_label_->setText(QString::number(threshold_value_));

			// When the slider is moved, we want to update the preview image if it is currently shown
			if (!current_image_mat_.empty() && !binarized_preview_image_mat_.empty() && isCurrentlyShowingPreview)
			{
				std::future<void> thread_handle = std::async(std::launch::async, &CreateBinaryImagePreview, std::ref(*this), std::ref(current_image_mat_), true);
			}
		});

	connect(this, &CalibrateWidget::UpdatePreviewMat, this, [this]()
		{
			DisplayPreviewMat();
		});

	connect(select_file_button_.get(), &QPushButton::clicked, this, [this]() {
		file_select_->show();
		});

	connect(file_select_.get(), &QFileDialog::fileSelected, this, [this](const QString& file) {
		qDebug() << "Selected file name: " << file;
		// Check if the selected file is a valid image format
		if (!file.contains(".jpg", Qt::CaseInsensitive) && !file.contains(".png", Qt::CaseInsensitive)) {
			MessageBoxHelper::ShowErrorDialog("Selected file was not in a valid image format (.jpg or .png)");
			return;
		}
		selected_image_filename_ = file;
		current_image_mat_ = imread(selected_image_filename_.toStdString(), IMREAD_GRAYSCALE);
		if (current_image_mat_.empty())
		{
			MessageBoxHelper::ShowErrorDialog("An error occurred while attempting to display the image file");
			return;
		}

		QPixmap image(file);
		QPixmap scaled = image.scaled(QSize(IMAGE_WIDTH, IMAGE_HEIGHT));
		original_image_->setPixmap(scaled);

		// Binarize the selected image on a separate thread to have it ready for preview
		auto thread_handle = std::async(std::launch::async, &CreateBinaryImagePreview, std::ref(*this), std::ref(current_image_mat_), false);
		});

	// When the preview button is pressed, we only want to binarize the input image for previewing (don't perform any calibration work yet)
	connect(preview_btn_.get(), &QPushButton::clicked, this, [this]() {
		isCurrentlyShowingPreview = true;
		DisplayPreviewMat();
		});

	connect(calibrate_btn_.get(), &QPushButton::clicked, this, [this]() {
		if (current_image_mat_.empty() || binarized_preview_image_mat_.empty())
		{
			MessageBoxHelper::ShowErrorDialog("Select an input image first");
			return;
		}
		if (gauge_diameter_ <= 0)
		{
			MessageBoxHelper::ShowErrorDialog("Please input a valid gauge diameter value");
			return;
		}

		auto thread_handle = std::async(std::launch::async, [this]()
			{
				std::pair<unsigned long long, unsigned long long> num_on_off_pixels = BinarizeImageHelper::GetNumberOfOnAndOffPixels(binarized_preview_image_mat_);
				double calibration_factor = BinarizeImageHelper::GetCalibrationGaugeFactor(num_on_off_pixels.first, gauge_diameter_);
				emit this->OnCalibrationComplete(calibration_factor);
			});
		});

	connect(this, &CalibrateWidget::OnCalibrationComplete, this, [this](double calibrationFactor) {
		calibration_factor_ = calibrationFactor;
		QString unit_display = GetUnitSuffix(current_unit_selection_);
		calibration_factor_label_->setText("Calibration Factor: " + QString::number(calibration_factor_) + " " + unit_display);
		});
}

