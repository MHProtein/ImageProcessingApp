#include "SpaceFilters.h"
#include "Image.h"

void BoxFilter(Image* src, Image* dest, cv::Size size)
{
	auto srcImage = src->GetImage().clone();
	auto destImage = dest->GetImage().clone();
	cv::boxFilter(srcImage, destImage, srcImage.depth(), size);
	dest->GetImage() = destImage;
}

void GaussianBlur(Image* src, Image* dest, cv::Size size, float sigma)
{
	auto srcImage = src->GetImage().clone();
	srcImage.convertTo(srcImage, CV_32F);
	auto destImage = dest->GetImage().clone();
	cv::GaussianBlur(srcImage, destImage, size, sigma);
	dest->GetImage() = destImage;
	destImage.convertTo(destImage, CV_8U);
	dest->GetImage() = destImage;
}

void MinFilter(Image* src, Image* dest, cv::Size size)
{
	auto srcImage = src->GetImage().clone();
	auto destImage = dest->GetImage().clone();
	cv::erode(srcImage, destImage, cv::getStructuringElement(cv::MORPH_RECT, size));
	dest->GetImage() = destImage;
}

void MedianFilter(Image* src, Image* dest, cv::Size size)
{
	auto srcImage = src->GetImage().clone();
	auto destImage = dest->GetImage().clone();
	cv::medianBlur(srcImage, destImage, size.width);
	dest->GetImage() = destImage;
}

void MaxFilter(Image* src, Image* dest, cv::Size size)
{
	auto srcImage = src->GetImage().clone();
	auto destImage = dest->GetImage().clone();
	cv::dilate(srcImage, destImage, cv::getStructuringElement(cv::MORPH_RECT, size));
	dest->GetImage() = destImage;
}

void Laplacian8(Image* src, Image* dest)
{
	auto srcImage = src->GetImage().clone();
	auto destImage = dest->GetImage().clone();
	cv::Laplacian(srcImage, destImage, CV_32F, 3);
	destImage.convertTo(destImage, CV_8U);
	dest->GetImage() = destImage;
	dest->Normalize();
}

void Laplacian4(Image* src, Image* dest)
{
	auto srcImage = src->GetImage().clone();
	auto destImage = dest->GetImage().clone();
	cv::Mat laplace4 = (cv::Mat_<float>(3, 3) <<
		0, -1, 0,
		-1, 4, -1,
		0, -1, 0);
	cv::filter2D(srcImage, destImage, CV_32F, laplace4);
	destImage.convertTo(destImage, CV_8U);
	dest->GetImage() = destImage;
	dest->Normalize();
}

void Sobel(Image* src, Image* dest, int dx, int dy)
{
	auto srcImage = src->GetImage().clone();
	auto destImage = dest->GetImage().clone();
	cv::Sobel(srcImage, destImage, CV_32F, dx, dy);
	destImage.convertTo(destImage, CV_8U);
	dest->GetImage() = destImage;

	dest->Normalize();
}

void Roberts(Image* src, Image* dest)
{
	auto srcImage = src->GetImage().clone();
	auto destImage = dest->GetImage().clone();
	cv::Mat Gx = (cv::Mat_<float>(2, 2) << 1, 0, 0, -1);
	cv::Mat Gy = (cv::Mat_<float>(2, 2) << 0, 1, -1, 0);
	cv::Mat gradX, gradY;
	cv::filter2D(srcImage, gradX, CV_32F, Gx);
	cv::filter2D(srcImage, gradY, CV_32F, Gy);

	cv::magnitude(gradX, gradY, destImage);

	destImage.convertTo(destImage, CV_8U);
	dest->GetImage() = destImage;
}