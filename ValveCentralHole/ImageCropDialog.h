#pragma once
#include <QDialog>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include "CameraConfirmLabel.h"
#include "ui_ImageCropDialog.h"
#include <condition_variable>
#include <future>


using namespace cv;


constexpr int LABEL_WIDTH = 1050;
constexpr int LABEL_HEIGHT = 600;

class ImageCropDialog : public QDialog
{
	Q_OBJECT
private:
	std::unique_ptr<CameraConfirmLabel> input_image_label_;
	std::unique_ptr<QPushButton> ok_btn_;
	Mat input_image_;
	bool is_crop_enabled = true;
	QRect current_cropped_region;
	std::function<void(const QString&)> confirm_callback_;
	bool isImageReadyToBeDisplayed = false;
	std::mutex mutex_;
	std::condition_variable cv_;

	Ui::ImageCropDialog ui;

	static QString temp_image_filename;
public:
	ImageCropDialog(Mat input_image, QWidget* parent = nullptr);
	void SetConfirmCallback(const std::function<void(const QString&)>& callback);

signals:
	void CanReadImageFile();
};

