#include "Image.h"

#include "MatUtils.h"

Image::Image(int index) : index(index)
{
	image = cv::Mat();
	image.copyTo(originalImage);
}

Image::Image(cv::Mat image, int index) : index(index)
{
	image.copyTo(this->image);
	image.copyTo(originalImage);
}

Image::Image(std::string filename, int flags, int index) : index(index), filename(filename)
{
	image = cv::imread(filename, flags);
	image.copyTo(originalImage);
}

bool Image::SaveImage(std::string path)
{
	if(image.empty())
		return false;

	if(cv::imwrite(path, image))
	{
		return true;
	}
	return false;
}

cv::Mat& Image::GetImage()
{
	return image;
}

const cv::Mat& Image::GetOriginalImage()
{
	return originalImage;
}

void Image::ResetImageToOriginal()
{
	image = originalImage.clone();
}

QImage Image::ToQImage()
{
	return MatUtils::MatToQImage(image);
}

void Image::Add(Image* rhs, Image* dst)
{
	auto lhsImage = image.clone();
	auto rhsImage = rhs->GetImage().clone();
	auto dstImage = dst->GetImage().clone();
	cv::add(lhsImage, rhsImage, dstImage);
	dst->GetImage() = dstImage;
	dst->Normalize();
}

void Image::Subtract(Image* rhs, Image* dst)
{
	auto lhsImage = image.clone();
	auto rhsImage = rhs->GetImage().clone();
	auto dstImage = dst->GetImage().clone();
	cv::subtract(lhsImage, rhsImage, dstImage);
	dst->GetImage() = dstImage;
	dst->Normalize();
}

void Image::Multiply(Image* rhs, Image* dst)
{
	auto lhsImage = image.clone();
	auto rhsImage = rhs->GetImage().clone();
	auto dstImage = dst->GetImage().clone();
	cv::multiply(lhsImage, rhsImage, dstImage);
	dst->GetImage() = dstImage;
	dst->Normalize();
}

void Image::Divide(Image* rhs, Image* dst)
{
	auto lhsImage = image.clone();
	auto rhsImage = rhs->GetImage().clone();
	auto dstImage = dst->GetImage().clone();
	cv::divide(lhsImage, rhsImage, dstImage);
	dst->GetImage() = dstImage;
	dst->Normalize();
}

void Image::Normalize()
{
	cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
}

void Image::GetNormalize(Image& dst) const
{
	cv::normalize(image, dst.GetImage(), 0, 255, cv::NORM_MINMAX);
}

void Image::Resize(int cols, int rows)
{
	cv::resize(image, image, cv::Size(cols, rows));
}

void Image::GetResize(int cols, int rows, Image* dst)
{
	cv::resize(image, dst->GetImage(), cv::Size(cols, rows));
}


void Image::GetLogTransformation(Image* dst) const
{
	auto img = dst->GetImage().clone();
	LogTransformationLogic(img);
	dst->GetImage() = img;
}

void Image::GetGammaTransformation(float gamma, Image* dst) const
{
	auto img = dst->GetImage().clone();
	GammaTransformationLogic(gamma, img);
	dst->GetImage() = img;
}

void Image::GetHistogramEqualization(Image* dst) const
{
	auto img = dst->GetImage().clone();
	HistogramEqualizationLogic(img);
	dst->GetImage() = img;
}

void Image::GetHistogramSpecification(Image* ref, Image* dst) const
{
	auto img = dst->GetImage().clone();
	auto refIMG = ref->GetImage().clone();
	HistogramSpecificationLogic(refIMG, img);
	dst->GetImage() = img;
}

void Image::LogTransformationLogic(cv::Mat& logImage) const
{
	cv::Mat floatImage;

	if (image.type() != CV_32F)
		image.convertTo(floatImage, CV_32F);
	MatUtils::Normalize(floatImage, 0, 255);

	cv::log(1 + floatImage, logImage);

	logImage -= 1;
	MatUtils::Normalize(logImage, 0, 255);
	logImage.convertTo(logImage, CV_8U);
}

void Image::GammaTransformationLogic(float gamma, cv::Mat& gammaImage) const
{
	cv::Mat floatImage;

	if (image.type() != CV_32F)
		image.convertTo(floatImage, CV_32F);
	MatUtils::Normalize(floatImage, 0, 255);

	cv::pow(floatImage, gamma, floatImage);

	MatUtils::Normalize(floatImage, 0, 255);
	floatImage.convertTo(gammaImage, CV_8U);
}

void Image::HistogramEqualizationLogic(cv::Mat& equalizedImage) const
{
	if (image.type() != CV_8U)
		image.convertTo(equalizedImage, CV_8U);
	else
		image.copyTo(equalizedImage);

	cv::Mat hist;
	MatUtils::GetHistogram(hist, equalizedImage);

	cv::Mat cdf;

	MatUtils::GetCDF(hist, cdf);

	MatUtils::Normalize(cdf, 0, 255);

	for (int i = 0; i != equalizedImage.rows; ++i)
	{
		for (int j = 0; j != equalizedImage.cols; ++j)
		{
			int grayValue = equalizedImage.at<uchar>(i, j);
			equalizedImage.at<uchar>(i, j) = static_cast<uchar>(std::round(cdf.at<float>(grayValue)));
		}
	}
}

void Image::HistogramSpecificationLogic(cv::Mat& imageToMatch, cv::Mat& matchedImage) const
{
	cv::Mat histOri;
	cv::Mat cdfOri;

	cv::Mat histRef;
	cv::Mat cdfRef;

	auto temp = image.clone();

	MatUtils::GetHistogram(histOri, temp);
	MatUtils::GetHistogram(histRef, imageToMatch);

	MatUtils::GetCDF(histOri, cdfOri);
	MatUtils::GetCDF(histRef, cdfRef);

	MatUtils::Normalize(cdfOri, 0, 1);
	MatUtils::Normalize(cdfRef, 0, 1);

	cv::Mat lut(1, 256, CV_8U);
	int j = 0;
	for (int i = 0; i < histOri.rows; ++i)
	{
		while (j < histOri.rows && cdfRef.at<float>(j) < cdfOri.at<float>(i))
			++j;
		lut.at<uchar>(i) = j;
	}

	cv::LUT(temp, lut, matchedImage);
}

