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
static void CreateBinaryImagePreview(CalibrateWidget& calibrate_widget, const Mat& original_mat, int threshold_value, int threshold_mode_selection_index, bool is_update, std::mutex& mutex) {
	Mat temp = original_mat.clone();

	ThresholdMode selected_threshold_mode = static_cast<ThresholdMode>(threshold_mode_selection_index);

	BinarizeImageHelper::BinarizeImage(temp, threshold_value, ThresholdMode::INVERTED);

	if (threshold_mode_selection_index == 0)
	{
		BinarizeImageHelper::InvertBinaryImage(temp);
	}

	// Acquire the mutex lock to avoid race condition where two threads set the binary preview mat at the same time
	{
		std::unique_lock<std::mutex> lock(mutex);
		calibrate_widget.SetPreviewMat(temp);
	}

	// Check if the image mat is extremely large (5000+ pixels in both directions)
	// If it is, we should only update when the sliders stops moving, or it'll lag too much
	if (temp.rows >= 5000 && temp.cols >= 5000)
	{
		// If the thread pool's queue is empty, this means that this working thread performed the last preview mat update so it should be updated in the GUI
		if (calibrate_widget.GetWidgetThreadPool().getTaskQueue().empty())
		{
			QMetaObject::invokeMethod(&calibrate_widget, [&calibrate_widget, temp, is_update]() mutable
				{
					if (is_update)
					{
						emit calibrate_widget.UpdatePreviewMat();
					}
				});
		}
	}
	else
	{
		QMetaObject::invokeMethod(&calibrate_widget, [&calibrate_widget, temp, is_update]() mutable
			{
				if (is_update)
				{
					emit calibrate_widget.UpdatePreviewMat();
				}
			});
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


CalibrateWidget::CalibrateWidget(const std::unique_ptr<bool>& gauge_helper_flag, QWidget* parent) : gauge_helper_flag_(gauge_helper_flag), QWidget(parent), ui(new Ui::CalibrateWidget) {

	ui->setupUi(this);

	// Initialize thread pool with 3 threads, which will be used for updating the binary preview image
	tp_.setNumThreads(3);

	InitializeUIElements();
	ConnectEventListeners();
}

void CalibrateWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPen pen(QBrush(QColor(255, 0, 0)), 10);
	painter.setPen(pen);

	for (auto& line_pair : line_points_)
	{
		// If the pair of points differ in their y coordinates, make the second point match the first's y coordinate
		// This is to make it so only straight lines are drawn
		if (line_pair.first.y() != line_pair.second.y())
		{
			QPointF& second_point = line_pair.second;
			second_point.setY(line_pair.first.y());
		}
		painter.drawLine(line_pair.first, line_pair.second);
	}
}


void CalibrateWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (*gauge_helper_flag_)
	{
		qDebug() << "Drawing line at (" << event->position().x() << ", " << event->position().y() << ")";
		if (is_on_line_start_)
		{
			start_line_ = event->position();
		}
		else
		{
			end_line_ = event->position();
			current_line_point_pair_ = std::make_pair(start_line_, end_line_);
			line_points_.push_back(current_line_point_pair_);
			update();
		}
		is_on_line_start_ = !is_on_line_start_;
	}
	QWidget::mouseDoubleClickEvent(event);

}


void CalibrateWidget::SetPreviewMat(Mat mat)
{
	binarized_preview_image_mat_ = mat;
}

const ThreadPool& CalibrateWidget::GetWidgetThreadPool() const
{
	return tp_;
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
	threshold_mode_combo_box_.reset(ui->threshold_mode_selection);
	is_saline_checkbox_.reset(ui->is_saline_selection);
	threshold_value_label_.reset(ui->threshold_value_display);
	select_file_button_.reset(ui->select_image_file);
	original_image_.reset(ui->original_image);
	binarized_image_.reset(ui->binarized_image);
	preview_btn_.reset(ui->preview_btn);
	calibrate_btn_.reset(ui->calibrate_btn);
	calibration_factor_label_.reset(ui->calibration_factor_label);
	threshold_mode_tooltip_label_.reset(ui->threshold_mode_tooltip_);
	saline_tooltip_label_.reset(ui->saline_tooltip);
	clear_lines_btn_.reset(ui->clear_lines);

	file_select_ = std::make_unique<QFileDialog>(this, "Select Gauge Image");

	diameter_unit_selection_->addItem("mm");
	diameter_unit_selection_->addItem("in");

	threshold_input_slider_->setMinimum(0);
	threshold_input_slider_->setMaximum(255);
	threshold_input_slider_->setSingleStep(1);
	threshold_input_slider_->setValue(127);

	threshold_mode_combo_box_->addItem("Standard");
	threshold_mode_combo_box_->addItem("Inverted");

	diameter_input_->setMinimum(0);
	diameter_input_->setMaximum(1000000);
	diameter_input_->setDecimals(5);

	file_select_->setStyleSheet("background: white;");

	threshold_mode_tooltip_label_->setPixmap(QPixmap("tooltip_question_mark.png")
		.scaled(threshold_mode_tooltip_label_->width(), threshold_mode_tooltip_label_->height()));
	saline_tooltip_label_->setPixmap(QPixmap("tooltip_question_mark.png")
		.scaled(saline_tooltip_label_->width(), saline_tooltip_label_->height()));

	threshold_mode_tooltip_label_->setToolTip("Choose the thresholding mode. \nIn Standard Thresholding mode, pixels with intensity values over the threshold value will become white, and intensity values below will be black."
		"\nIn Inverted, the inverse is applied");
	saline_tooltip_label_->setToolTip("Check the box to indicate that the calculations should take into account the index of refraction for saline."
		"\nIn particular, it will account for the magnification cause by refraction");

	threshold_mode_tooltip_label_->setToolTipDuration(20000);
	saline_tooltip_label_->setToolTipDuration(20000);
}

void CalibrateWidget::DisplayCalibrationFactor()
{
	QString unit_display = GetUnitSuffix(current_unit_selection_);
	calibration_factor_label_->setText("Calibration Factor: " + QString::number(calibration_factor_) + " " + unit_display);
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
				tp_.enqueue([this, value]()
					{
						CreateBinaryImagePreview(*this, current_image_mat_, value, threshold_mode_combo_box_->currentIndex(), true, mutex_);
					});
			}
		});

	connect(threshold_mode_combo_box_.get(), &QComboBox::currentIndexChanged, this, [this](int index)
		{
			if (!binarized_preview_image_mat_.empty())
			{
				BinarizeImageHelper::InvertBinaryImage(binarized_preview_image_mat_);
			}
			if (isCurrentlyShowingPreview)
			{
				DisplayPreviewMat();
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
		auto thread_handle = std::async(std::launch::async, &CreateBinaryImagePreview,
			std::ref(*this), std::ref(current_image_mat_), threshold_value_, threshold_mode_combo_box_->currentIndex(), false, std::ref(mutex_));
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
				double calibration_factor;
				switch (threshold_mode_combo_box_->currentIndex())
				{
					// Standard thresholding mode, we should use the number of off pixels to get the calibration factor
				case 0:
				{
					calibration_factor = BinarizeImageHelper::GetCalibrationGaugeFactor(num_on_off_pixels.second, gauge_diameter_);
					break;
				}
				case 1:
				{
					calibration_factor = BinarizeImageHelper::GetCalibrationGaugeFactor(num_on_off_pixels.first, gauge_diameter_);
					break;
				}
				}

				// Apply any necessary saline transformations to the resulting calibration factor
				if (is_saline_checkbox_->isChecked())
				{
					calibration_factor = BinarizeImageHelper::ApplySalineTransformation(calibration_factor);
				}

				emit this->OnCalibrationComplete(calibration_factor);
			});
		});

	connect(this, &CalibrateWidget::OnCalibrationComplete, this, [this](double calibrationFactor) {
		calibration_factor_ = calibrationFactor;
		DisplayCalibrationFactor();
		});

	connect(clear_lines_btn_.get(), &QPushButton::clicked, this, [this]()
		{

		});
}

