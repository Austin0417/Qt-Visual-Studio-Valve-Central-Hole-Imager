#include "ImageCropDialog.h"

#include "MessageBoxHelper.h"


QString ImageCropDialog::temp_image_filename = "temp_image.jpg";

ImageCropDialog::ImageCropDialog(Mat input_image, QWidget* parent) : input_image_(input_image), QDialog(parent)
{
	ui.setupUi(this);

	// Resize the input Mat dimensions so that it fits with LABEL_WIDTH and LABEL_HEIGHT
	// Determine whether the image will be shrunk, or enlarged

	// Two threads, one thread will perform the mat resize and write the result to a temporary image file, and the other thread will open the image file to display on the pixmap when ready
	auto thread_one_handle = std::async(std::launch::async, [this]()
		{
			// Resized image is larger, enlarging occurs
			if (LABEL_WIDTH * LABEL_HEIGHT > input_image_.rows * input_image_.cols)
			{
				cv::resize(input_image_, input_image_, Size(LABEL_WIDTH, LABEL_HEIGHT), 0, 0, INTER_CUBIC);
			}
			// Resized image is smaller, shrinking occurs
			else if (LABEL_WIDTH * LABEL_HEIGHT < input_image_.rows * input_image_.cols)
			{
				cv::resize(input_image_, input_image_, Size(LABEL_WIDTH, LABEL_HEIGHT), 0, 0, INTER_AREA);
			}

			if (imwrite(temp_image_filename.toStdString(), input_image_))
			{
				isImageReadyToBeDisplayed = true;
				cv_.notify_all();
			}
		});

	// This thread waits until the other finishes resizing and saving the image file before sending a signal to the main GUI thread to set the pixmap
	auto thread_two_handle = std::async(std::launch::async, [this]()
		{
			{
				std::unique_lock<std::mutex> lock(mutex_);
				cv_.wait(lock, [this]()
					{
						return isImageReadyToBeDisplayed;
					});
			}
			emit this->CanReadImageFile();
		});


	input_image_label_ = std::make_unique<CameraConfirmLabel>(is_crop_enabled, this);
	input_image_label_->setFixedWidth(LABEL_WIDTH);
	input_image_label_->setFixedHeight(LABEL_HEIGHT);
	input_image_label_->move(80, 30);
	input_image_label_->setScaledContents(true);
	input_image_label_->SetCropCallback([this](int x, int y, int width, int height)
		{
			current_cropped_region = QRect(QPoint(x, y), QPoint(x + std::abs(width), y + std::abs(height)));
		});
	ok_btn_.reset(ui.image_crop_ok);

	connect(ok_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			// Check for the cases where the user did not crop the image or the callback was never set for this dialog
			if (!confirm_callback_ || current_cropped_region.isNull())
			{
				MessageBoxHelper::ShowErrorDialog("Failed to crop image (callback was never set, or no region was selected to be cropped");
				return;
			}

			// When the user clicks confirm, we will take the cropped region out from the original input mat and write this to the same temporary image file
			input_image_ = input_image_(Rect(current_cropped_region.x(), current_cropped_region.y(), current_cropped_region.width(), current_cropped_region.height()));
			if (!imwrite(temp_image_filename.toStdString(), input_image_))
			{
				MessageBoxHelper::ShowErrorDialog("An error occurred while trying to crop the image, please try again");
				return;
			}
			confirm_callback_(temp_image_filename);
			this->close();
		});

	connect(this, &ImageCropDialog::CanReadImageFile, this, [this]()
		{
			input_image_label_->setPixmap(QPixmap(temp_image_filename).scaled(LABEL_WIDTH, LABEL_HEIGHT));
		});
}


void ImageCropDialog::SetConfirmCallback(const std::function<void(const QString&)>& callback)
{
	confirm_callback_ = callback;
}
