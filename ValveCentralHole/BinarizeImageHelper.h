#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <utility>
#include <future>
#include <string>
#include <vector>

#define TEMP_FILENAME "preview_temp.jpg"
constexpr double PI = 3.14159;

using namespace cv;

enum class ThresholdMode {
	STANDARD,
	INVERTED
};

class BinarizeImageHelper
{
public:
	BinarizeImageHelper();
	static void BinarizeImage(Mat& input, int threshold_value, ThresholdMode mode = ThresholdMode::STANDARD);
	static std::pair<unsigned long long, unsigned long long> GetNumberOfOnAndOffPixels(Mat& input);
	static double GetCalibrationGaugeFactor(unsigned long long num_on_pixels, double gauge_diameter);
	static double CalculateValveArea(unsigned long long number_on_pixels, double calibration_factor);
	static void InvertBinaryImage(Mat& mat);
	static double ApplySalineTransformation(double calibration_factor);
};
