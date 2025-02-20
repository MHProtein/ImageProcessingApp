#pragma once
#ifndef MAT_UTILS_H
#define MAT_UTILS_H
#include <opencv2/opencv.hpp>

#include "Image.h"

class MatUtils
{
public:

	static void Normalize(cv::Mat& img, int min, int max);
	static void Normalize(cv::Mat& src, cv::Mat& dst, int min, int max);

	static void GetHistogram(cv::Mat& hist, cv::Mat& img);
	static void GetCDF(cv::Mat& hist, cv::Mat& cdf);

	static QImage MatToQImage(cv::Mat& mat);

	static cv::Mat FFT(cv::Mat& image);
	static cv::Mat ShiftFFT(cv::Mat& complexI);
	static cv::Mat GetMagnitudeSpectrum(cv::Mat& complexI);

};

#endif // MAT_UTILS_H