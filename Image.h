#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <opencv2/opencv.hpp>

#include "ImageListItemContainerWidget.h"

class Image
{
public:
	Image(int index = -1);
	Image(cv::Mat image, int index = -1);
	Image(std::string filename, int flags = cv::IMREAD_COLOR, int index = -1);

	bool SaveImage(std::string path);

	cv::Mat& GetImage();
	const cv::Mat& GetOriginalImage();
	void ResetImageToOriginal();

	auto GetSize() { return image.size; }
	int GetIndex() { return index; }
	int GetWidth() { return image.cols; }
	int GetHeight() { return image.rows; }

	QImage ToQImage();

	template<class T>
	T GetPixel(int x, int y);

	void Add(Image* rhs, Image* dst);
	void Subtract(Image* rhs, Image* dst);
	void Multiply(Image* rhs, Image* dst);
	void Divide(Image* rhs, Image* dst);

	void Normalize();
	void GetNormalize(Image& dst) const;

	void Resize(int cols, int rows);
	void GetResize(int cols, int rows, Image* dst);

	void GetLogTransformation(Image* dst) const;

	void GetGammaTransformation(float gamma, Image* dst) const;

	void GetHistogramEqualization(Image* dst) const;

	void GetHistogramSpecification(Image* ref, Image* dst) const;

private:
	void LogTransformationLogic(cv::Mat& logImage) const;
	void GammaTransformationLogic(float gamma, cv::Mat& gammaImage) const;
	void HistogramEqualizationLogic(cv::Mat& equalizedImage) const;
	void HistogramSpecificationLogic(cv::Mat& imageToMatch, cv::Mat& matchedImage) const;

	int index;
	cv::Mat image;
	cv::Mat originalImage;
	std::string filename;
};

template <class T>
T Image::GetPixel(int x, int y)
{
	return image.at<T>(y, x);
}

#endif // IMAGE_H