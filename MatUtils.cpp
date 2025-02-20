#include "MatUtils.h"

#include "Image.h"

void MatUtils::Normalize(cv::Mat& img, int min, int max)
{
	cv::normalize(img, img, min, max, cv::NORM_MINMAX);
}

void MatUtils::Normalize(cv::Mat& src, cv::Mat& dst, int min, int max)
{
	cv::normalize(src, dst, min, max, cv::NORM_MINMAX);
}

void MatUtils::GetHistogram(cv::Mat& hist, cv::Mat& img)
{
	int size = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	cv::calcHist(&img, 1, 0, cv::Mat(), hist, 1, &size, &histRange);
}

void MatUtils::GetCDF(cv::Mat& hist, cv::Mat& cdf)
{
	cdf = cv::Mat(hist.size(), hist.type());
	cdf.at<float>(0) = hist.at<float>(0);
	for (int i = 1; i != hist.rows; ++i)
	{
		cdf.at<float>(i) = hist.at<float>(i) + cdf.at<float>(i - 1);
	}
}

QImage MatUtils::MatToQImage(cv::Mat& image)
{
	return QImage(image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8);
}

cv::Mat MatUtils::FFT(cv::Mat& image)
{
	cv::Mat padded;

	int m = cv::getOptimalDFTSize(image.rows);
	int nSize = cv::getOptimalDFTSize(image.cols);
	cv::copyMakeBorder(image, padded, 0, m - image.rows, 0, nSize - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
	cv::Mat complexI;
	cv::merge(planes, 2, complexI);
	cv::dft(complexI, complexI);
    
    return ShiftFFT(complexI);
}

cv::Mat MatUtils::ShiftFFT(cv::Mat& complexI)
{
	auto temp = complexI.clone();
    int cx = temp.cols / 2;
    int cy = temp.rows / 2;

    cv::Mat q0(temp, cv::Rect(0, 0, cx, cy));  
    cv::Mat q1(temp, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(temp, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(temp, cv::Rect(cx, cy, cx, cy));

    cv::Mat tmp;
    
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
  
    return temp;
}

cv::Mat MatUtils::GetMagnitudeSpectrum(cv::Mat& complexI)
{
    cv::Mat magnitudeImage;
    cv::Mat planes[2];
    cv::split(complexI, planes);
    cv::magnitude(planes[0], planes[1], magnitudeImage);

    magnitudeImage += cv::Scalar::all(1);
    cv::log(magnitudeImage, magnitudeImage);

    magnitudeImage.convertTo(magnitudeImage, CV_8U);
    cv::normalize(magnitudeImage, magnitudeImage, 0, 255, cv::NORM_MINMAX);

    return magnitudeImage;
}
