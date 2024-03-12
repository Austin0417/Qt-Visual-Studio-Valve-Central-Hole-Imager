#include "ImageCropDialog.h"
#include "BinarizeImageHelper.h"
#include "MessageBoxHelper.h"



QString ImageCropDialog::temp_image_filename = "temp_image.jpg";


// TODO Apply the new crop translation technique
ImageCropDialog::ImageCropDialog(Mat input_image, QWidget* parent) : input_image_(input_image), QDialog(parent)
{
	ui.setupUi(this);

	// Resize the input Mat dimensions so that it fits with LABEL_WIDTH and LABEL_HEIGHT
	// Determine whether the image will be shrunk, or enlarged

	auto thread_one = std::async(std::launch::async, [this]()
		{
			Mat temp = input_image_.clone();
			cv::resize(temp, temp, Size(LABEL_WIDTH, LABEL_HEIGHT));
			emit this->CanSetPixmap(temp);
		});


	input_image_label_ = std::make_unique<CameraConfirmLabel>(is_crop_enabled, this);
	input_image_label_->setFixedWidth(LABEL_WIDTH);
	input_image_label_->setFixedHeight(LABEL_HEIGHT);
	input_image_label_->move(80, 30);
	input_image_label_->setScaledContents(true);
	input_image_label_->SetCropCallback([this](int x, int y, int width, int height)
		{
			BinarizeImageHelper::ApplyImageCroppingFromQLabel(input_image_, cropped_, x, y, std::abs(width), std::abs(height), input_image_label_->width(), input_image_label_->height());
		});

	ok_btn_.reset(ui.image_crop_ok);

	connect(ok_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			// Check for the cases where the user did not crop the image or the callback was never set for this dialog
			if (!confirm_callback_ || cropped_.empty())
			{
				MessageBoxHelper::ShowErrorDialog("Failed to crop image (callback was never set, or no region was selected to be cropped");
				return;
			}

			// When the user clicks confirm, we will take the cropped region out from the original input mat and write this to the same temporary image file
			confirm_callback_(cropped_);
			this->close();
		});

	connect(this, &ImageCropDialog::CanSetPixmap, this, [this](const Mat& mat)
		{
			QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format::Format_Grayscale8);
			QPixmap pixmap = QPixmap::fromImage(image).scaled(LABEL_WIDTH, LABEL_HEIGHT);
			input_image_label_->setPixmap(pixmap);
		});
}


void ImageCropDialog::SetConfirmCallback(const std::function<void(const Mat& mat)>& callback)
{
	confirm_callback_ = callback;
}
