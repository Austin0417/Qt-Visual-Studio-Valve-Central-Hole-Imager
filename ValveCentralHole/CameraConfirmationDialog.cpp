#include "CameraConfirmationDialog.h"
#include "CameraDisplayDialog.h"
#include "MessageBoxHelper.h"


CameraConfirmationDialog::CameraConfirmationDialog(Mat captured_mat, QWidget* parent) : captured_mat_(captured_mat),
QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle("Confirm Image");
	setWindowIcon(QIcon("window_icon.jpg"));
	InitializeUiElements();
	ConnectEventListeners();
}


void CameraConfirmationDialog::InitializeUiElements()
{
	crop_btn_.reset(ui.camera_crop_btn);
	displayed_mat_label_ = std::make_unique<CameraConfirmLabel>(is_crop_enabled_, this);
	retake_btn_.reset(ui.camera_retake_btn);
	confirm_btn_.reset(ui.camera_confirm_btn);
	clear_crop_btn.reset(ui.clear_crop_lines_btn);
	image_name_input_.reset(ui.captured_image_name_line_edit);

	crop_btn_->setIcon(QIcon("crop_icon.png"));
	crop_btn_->setCheckable(true);

	displayed_mat_label_->setFixedWidth(1050);
	displayed_mat_label_->setFixedHeight(600);
	displayed_mat_label_->move(80, 30);

	// Callback that will be called everytime the user crops the image
	displayed_mat_label_->SetCropCallback([this](int x, int y, int width, int height)
		{
			// When the user crops the image, we will store the resulting sub matrix
			cropped_captured_mat_ = captured_mat_(Rect(x, y, std::abs(width), std::abs(height)));
		});

	displayed_mat_label_->setPixmap(QPixmap::fromImage(QImage(captured_mat_.data, captured_mat_.cols,
		captured_mat_.rows, QImage::Format_BGR888)).scaled(DIALOG_WIDTH, DIALOG_HEIGHT));
}


void CameraConfirmationDialog::ConnectEventListeners()
{
	connect(crop_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			is_crop_enabled_ = crop_btn_->isChecked();
		});

	connect(retake_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (on_retake_requested_callback_)
			{
				on_retake_requested_callback_();
				close();
			}
		});

	connect(clear_crop_btn.get(), &QPushButton::clicked, this, [this]()
		{
			displayed_mat_label_->ClearCropLines();
		});

	connect(confirm_btn_.get(), &QPushButton::clicked, this, [this]()
		{
			if (image_name_input_->text().isEmpty())
			{
				MessageBoxHelper::ShowErrorDialog("Image file name cannot be empty");
				return;
			}

			if (on_confirm_callback_)
			{
				if (!cropped_captured_mat_.empty())
				{
					on_confirm_callback_(cropped_captured_mat_, image_name_input_->text() + ".jpg");
				}
				else
				{
					on_confirm_callback_(captured_mat_, image_name_input_->text());
				}
			}
			close();
		});
}


void CameraConfirmationDialog::SetRetakeCallback(const std::function<void()>& callback)
{
	on_retake_requested_callback_ = callback;
}


void CameraConfirmationDialog::SetConfirmCallback(const std::function<void(Mat, const QString&)>& callback)
{
	on_confirm_callback_ = callback;
}
