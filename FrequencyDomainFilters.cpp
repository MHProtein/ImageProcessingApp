#include "FrequencyDomainFilters.h"

#include "MatUtils.h"

cv::Mat ApplyFilter(cv::Mat& complexI, const cv::Mat& filter, int alpha, int beta)
{
	cv::Mat iComplexI;
	cv::Mat planes[] = { cv::Mat_<float>(complexI), cv::Mat::zeros(complexI.size(), CV_32F) };
	cv::split(complexI, planes);
	planes[0] = planes[0].mul(filter);
	planes[1] = planes[1].mul(filter);

	cv::merge(planes, 2, complexI);

	iComplexI = MatUtils::ShiftFFT(complexI);

	cv::idft(iComplexI, iComplexI);

	cv::split(iComplexI, planes);
	cv::Mat magnitude;
	magnitude = planes[0];
	cv::normalize(magnitude, magnitude, alpha, beta, cv::NORM_MINMAX);
	return magnitude;
}

cv::Mat ComputePhaseSpectrum(const cv::Mat& complexI)
{
	cv::Mat planes[2];
	cv::split(complexI, planes);

	cv::Mat phase = cv::Mat::zeros(planes[0].size(), CV_32F);
	cv::phase(planes[0], planes[1], phase);

	cv::normalize(phase, phase, 0, 1, cv::NORM_MINMAX);
	return phase;
}

/// \brief 
/// \param type 0 - Ideal, 1 - Gaussian, 2 - Butterworth
void LowPassFilter(Image* image, Image* dest, Image* frequencyDest, double d0, int type, int n)
{
	auto complexI = MatUtils::FFT(image->GetImage());
	cv::Mat filter = cv::Mat::zeros(complexI.size(), CV_32F);
	for (int i = 0; i < complexI.rows; i++)
	{
		for (int j = 0; j < complexI.cols; j++)
		{
			double distance = sqrt(pow(i - complexI.rows / 2, 2) + pow(j - complexI.cols / 2, 2));
			if (type == 0)
			{
				filter.at<float>(i, j) = (distance <= d0) ? 1 : 0;
			}
			else if (type == 1)
			{
				filter.at<float>(i, j) = exp(-(distance * distance) / (2 * d0 * d0));
			}
			else if (type == 2)
			{
				filter.at<float>(i, j) = 1.0 / (1.0 + pow(distance / d0, 2 * n));
			}
		}
	}

	dest->GetImage() = ApplyFilter(complexI, filter);
	dest->GetImage().convertTo(dest->GetImage(), CV_8U, 255);
	if (!dest)
		return;
	auto magnitude = MatUtils::GetMagnitudeSpectrum(complexI);
	frequencyDest->GetImage() = magnitude.clone();
}

/// \brief 
/// \param type 0 - Ideal, 1 - Gaussian, 2 - Butterworth
void HighPassFilter(Image* image, Image* dest, Image* frequencyDest, double d0, int type, int n)
{
	auto complexI = MatUtils::FFT(image->GetImage());
	cv::Mat filter = cv::Mat::ones(complexI.size(), CV_32F);

	for (int i = 0; i < complexI.rows; i++) {
		for (int j = 0; j < complexI.cols; j++) {

			double u = i - complexI.rows / 2;
			double v = j - complexI.cols / 2;
			double distance = sqrt(u * u + v * v);

			if (type == 0) {
				filter.at<float>(i, j) = 1.0f - (distance <= d0) ? 1.0f : 0;
			}
			else if (type == 1) { 
				filter.at<float>(i, j) = 1.0f - exp(-(distance * distance) / (2.0f * d0 * d0));
			}
			else if (type == 2) { 
				filter.at<float>(i, j) = 1.0f - 1.0f / (1.0f + pow(distance / d0, 2.0f * n));
			}
		}
	}

	dest->GetImage() = ApplyFilter(complexI, filter);
	dest->GetImage().convertTo(dest->GetImage(), CV_8U, 255);
	if (!dest)
		return;
	auto magnitude = MatUtils::GetMagnitudeSpectrum(complexI);
	frequencyDest->GetImage() = magnitude.clone();
}

cv::Mat CreateHomomorphicFilter(cv::Size size, float d0, float gammaL, float gammaH, float c)
{
	cv::Mat filter(size, CV_32F, cv::Scalar(0));
	int rows = size.height;
	int cols = size.width;

	for (int u = 0; u < rows; u++) 
	{
		for (int v = 0; v < cols; v++)
		{
			float distance = std::sqrt(std::pow(u - rows / 2, 2) + std::pow(v - cols / 2, 2));
			filter.at<float>(u, v) = (gammaH - gammaL) * (1 - std::exp(-c * (distance * distance) / (d0 * d0))) + gammaL;
		}
	}
	return filter;
}

void ApplyHomomorphicFilter(Image* image, Image* dest, Image* frequencyDest, float d0, float gammaL,
	float gammaH, float c)
{
	cv::Mat logImage;
	image->GetImage().convertTo(logImage, CV_32F);
	logImage += 1;
	cv::log(logImage, logImage);

	cv::Mat complexI = MatUtils::FFT(logImage);

	cv::Mat homomorphicFilter = CreateHomomorphicFilter(complexI.size(), d0, gammaL, gammaH, c);

	auto result = ApplyFilter(complexI, homomorphicFilter, -1, 1);
	cv::exp(result, result);

	result -= 1;
	result = result(cv::Rect(0, 0, logImage.cols, logImage.rows));

	cv::normalize(result, result, 0, 255, cv::NORM_MINMAX);
	result.convertTo(result, CV_8U);
	if (!dest)
		return;
	dest->GetImage() = result;
	frequencyDest->GetImage() = MatUtils::GetMagnitudeSpectrum(complexI);
}

cv::Mat CreateNotchRejectFilter(cv::Size size, int u_k, int v_k, int d0)
{
	cv::Mat filter(size, CV_32F, cv::Scalar(1));

	int rows = size.height;
	int cols = size.width;

	for (int u = 0; u < rows; u++)
	{
		for (int v = 0; v < cols; v++)
		{
			float D1 = std::sqrt(std::pow(u - u_k, 2) + std::pow(v - v_k, 2));
			float D2 = std::sqrt(std::pow(u + u_k - rows, 2) + std::pow(v + v_k - cols, 2));

			filter.at<float>(u, v) = (D1 * D2) / ((D1 * D2) + d0 * d0);
		}
	}
	return filter;
}

void ApplyNotchRejectFilter(Image* image, Image* dest, Image* frequencyDest, int u_k, int v_k, int d0)
{
	auto complexI = MatUtils::FFT(image->GetImage());
	cv::Mat notchFilter = CreateNotchRejectFilter(complexI.size(), u_k, v_k, d0);

	cv::Mat result = ApplyFilter(complexI, notchFilter);
	result = result(cv::Rect(0, 0, image->GetWidth(), image->GetHeight()));
	result.convertTo(result, CV_8U, 255);

	if (!dest)
		return;
	dest->GetImage() = result;
	frequencyDest->GetImage() = MatUtils::GetMagnitudeSpectrum(complexI);
}
