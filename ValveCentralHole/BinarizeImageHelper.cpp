#include "BinarizeImageHelper.h"


BinarizeImageHelper::BinarizeImageHelper() {}



/**
 * \brief
 * \param input [in] : grayscale input matrix to binarize
 * \param threshold_value [in] : threshold value ranging from 0 - 255. In ThresholdMode::STANDARD, pixels with intensities above the threshold value will be white, and below will be off.
 * \param mode [in] : Input to determine thresholding mode. In inverted mode, pixel values above the threshold value will be off, and on if above (opposite of standard mode)
 */
void BinarizeImageHelper::BinarizeImage(Mat& input, int threshold_value, ThresholdMode mode) {
	for (int i = 0; i < input.rows; i++) {
		unsigned char* current_row = input.ptr(i);
		for (int j = 0; j < input.cols * input.channels(); j++) {
			switch (mode) {
			case ThresholdMode::STANDARD: {
				if (current_row[j] < threshold_value) {
					current_row[j] = 0;
				}
				else {
					current_row[j] = 255;
				}
			}
			case ThresholdMode::INVERTED: {
				if (current_row[j] < threshold_value) {
					current_row[j] = 255;
				}
				else {
					current_row[j] = 0;
				}
			}
			}
		}
	}

	// Fill holes
	int dilation_size = 1;
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * dilation_size + 1, 2 * dilation_size + 1));
	morphologyEx(input, input, MORPH_OPEN, element);
}

/**
 * \brief
 * \param input [in] :  Binary input matrix that we want to count the number of on and off pixels of (has to be a binary image)
 * \return : std::pair where the first value is the number of on pixels, and second is the number of off pixels
 */
std::pair<unsigned long long, unsigned long long> BinarizeImageHelper::GetNumberOfOnAndOffPixels(Mat& input) {
	std::pair<unsigned long long, unsigned long long> result;
	unsigned long long on_pixels = 0;
	unsigned long long off_pixels = 0;

	for (int i = 0; i < input.rows; i++) {
		unsigned char* current_row = input.ptr(i);
		for (int j = 0; j < input.cols * input.channels(); j++) {
			switch (current_row[j]) {
			case 0: {
				off_pixels++;
				break;
			}
			case 255: {
				on_pixels++;
				break;
			}
			}
		}
	}
	result = std::make_pair(on_pixels, off_pixels);
	return result;
}


/**
 * \brief
 * \param num_on_pixels [in] : total number of on pixels within the binary calibration gauge image
 * \param gauge_diameter [in] : gauge diameter of the calibration gauge
 * \return : the calibration gauge factor (how much area each pixel is in mm^2)
 */
double BinarizeImageHelper::GetCalibrationGaugeFactor(unsigned long long num_on_pixels, double gauge_diameter)
{
	double actual_gauge_area = PI * gauge_diameter * gauge_diameter * 0.25;
	return actual_gauge_area / num_on_pixels;
}

double BinarizeImageHelper::CalculateValveArea(unsigned long long number_on_pixels, double calibration_factor)
{
	return number_on_pixels * calibration_factor;
}

void BinarizeImageHelper::InvertBinaryImage(Mat& mat)
{
	bitwise_xor(mat, Scalar(255), mat);
}


double BinarizeImageHelper::ApplySalineTransformation(double calibration_factor)
{
	// TODO Figure out how to adjust for the saline index of refraction here


	return calibration_factor;
}

