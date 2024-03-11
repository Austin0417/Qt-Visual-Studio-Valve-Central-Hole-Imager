#pragma once
#include <QDialog>
#include <QScreen>
#include <QGuiApplication>
#include <QCheckBox>
#include "CameraConfirmLabel.h"
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <future>
#include <condition_variable>
#include <mutex>

using namespace cv;

class SelectImageCropDialog : public QDialog
{
	Q_OBJECT
private:
	QString image_file_name_;
	QString latest_directory_entered_;
	std::unique_ptr<CameraConfirmLabel> image_label_;
	std::unique_ptr<QCheckBox> save_as_new_file_checkbox_;
	std::unique_ptr<QLineEdit> new_image_file_name_;
	std::unique_ptr<QPushButton> reselect_image_btn_;
	std::unique_ptr<QPushButton> confirm_btn_;
	std::unique_ptr<QPushButton> reset_btn_;

	bool is_reselecting_image_ = false;
	std::unique_ptr<QFileDialog> file_dialog_;

	std::mutex mutex_;
	std::condition_variable cv_;
	bool is_resizing_complete_ = false;

	Mat mat_;
	Mat cropped_;

	void InitializeUIElements(QScreen* screen);
	void ConnectEventListeners();
	void SetPixmapForLabel();
	void SetPixmapForLabel(Mat mat, int x, int y, int width, int height);	// Sets a cropped mat for the pixmap with the given x, y, width, and height
public:
	SelectImageCropDialog(QString image_file_name, QWidget* parent = nullptr);

signals:
	void IsReadyToDisplayPixmap(Mat mat);
	void ShouldCloseDialog();
};

