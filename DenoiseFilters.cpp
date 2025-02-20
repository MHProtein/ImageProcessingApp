#include "DenoiseFilters.h"

void AddGaussianNoise(Image* image, Image* dst, float mean, float var)
{
	auto src = image->GetImage();
	cv::Mat noise = cv::Mat(src.size(), CV_32F);
	cv::randn(noise, mean, var);

	cv::Mat noisyImage;
	src.convertTo(noisyImage, CV_32F);
	noisyImage += noise;
	cv::normalize(noisyImage, noisyImage, 0, 255, cv::NORM_MINMAX);
	noisyImage.convertTo(noisyImage, CV_8U);
	dst->GetImage() = noisyImage;
}

void ArithmeticMeanFilter(Image* image, Image* dst, int kernelSize)
{
	auto src = image->GetImage();
	cv::Mat result;
	cv::blur(src, result, cv::Size(kernelSize, kernelSize));
	dst->GetImage() = result;
}

void GeometricMeanFilter(Image* image, Image* dst, int kernelSize)
{
	auto src = image->GetImage();
	cv::Mat srcLog, result;
	src.convertTo(srcLog, CV_32F);
	srcLog += 1;
	cv::log(srcLog, srcLog);

	cv::blur(srcLog, srcLog, cv::Size(kernelSize, kernelSize));
	cv::exp(srcLog, result);

	result.convertTo(result, CV_8U);
	dst->GetImage() = result;
}

void AdaptiveLocalNoiseReductionFilter(Image* image, Image* dst, int kernelSize, float noiseVariance)
{
	cv::Mat srcFloat;
	image->GetImage().convertTo(srcFloat, CV_32F);

	cv::Mat localMean, localVariance;
	cv::blur(srcFloat, localMean, cv::Size(kernelSize, kernelSize));

	cv::blur(srcFloat.mul(srcFloat), localVariance, cv::Size(kernelSize, kernelSize));
	localVariance -= localMean.mul(localMean);

	cv::Mat result = srcFloat - localMean;
	result = result.mul(noiseVariance / (localVariance + noiseVariance));
	result += localMean;

	result.convertTo(result, CV_8U);
	dst->GetImage() = result;
}

void AddSaltAndPepperNoise(Image* image, Image* dst, float saltProb, float pepperProb)
{
	cv::Mat noisyImage = image->GetImage().clone();
	int totalPixels = noisyImage.rows * noisyImage.cols;
	int saltCount = totalPixels * saltProb;
	int pepperCount = totalPixels * pepperProb;

	for (int i = 0; i < saltCount; i++)
	{
		int x = rand() % noisyImage.cols;
		int y = rand() % noisyImage.rows;
		noisyImage.at<uchar>(y, x) = 255;
	}

	for (int i = 0; i < pepperCount; i++)
	{
		int x = rand() % noisyImage.cols;
		int y = rand() % noisyImage.rows;
		noisyImage.at<uchar>(y, x) = 0;
	}
	dst->GetImage() = noisyImage;
}

void MedianFilter(Image* image, Image* dst, int kernelSize)
{
	cv::Mat result;
	cv::medianBlur(image->GetImage(), result, kernelSize);
	dst->GetImage() = result;
}

void AlphaTrimmedMeanFilter(Image* image, Image* dst, int kernelSize, int d)
{
	auto src = image->GetImage();
	cv::Mat result = image->GetImage().clone();
	int trimCount = d / 2;
	for (int y = trimCount; y < src.rows - trimCount; y++)
	{
		for (int x = trimCount; x < src.cols - trimCount; x++)
		{
			std::vector<uchar> neighborhood;
			for (int i = -trimCount; i <= trimCount; i++)
			{
				for (int j = -trimCount; j <= trimCount; j++)
				{
					neighborhood.push_back(src.at<uchar>(y + i, x + j));
				}
			}
			std::sort(neighborhood.begin(), neighborhood.end());
			int sum = 0;
			for (int k = trimCount; k < neighborhood.size() - trimCount; k++)
			{
				sum += neighborhood[k];
			}
			result.at<uchar>(y, x) = sum / (neighborhood.size() - d);
		}
	}
	dst->GetImage() = result;
}

void AdaptiveMedianFilter(Image* image, Image* dst, int maxKernelSize)
{
	auto src = image->GetImage();
	cv::Mat result = src.clone();
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++) 
		{
			int kernelSize = 3;
			bool done = false;
			while (!done && kernelSize <= maxKernelSize)
			{
				int halfSize = kernelSize / 2;
				std::vector<uchar> neighborhood;

				for (int i = -halfSize; i <= halfSize; i++) 
				{
					for (int j = -halfSize; j <= halfSize; j++)
					{
						neighborhood.push_back(src.at<uchar>(std::clamp(y + i, 0, src.rows - 1),
							std::clamp(x + j, 0, src.cols - 1)));
					}
				}

				std::sort(neighborhood.begin(), neighborhood.end());
				uchar zMin = neighborhood.front();
				uchar zMax = neighborhood.back();
				uchar zMed = neighborhood[neighborhood.size() / 2];
				uchar zXY = src.at<uchar>(y, x);

				if (zMed > zMin && zMed < zMax)
				{
					if (zXY > zMin && zXY < zMax) 
					{
						result.at<uchar>(y, x) = zXY;
					}
					else 
					{
						result.at<uchar>(y, x) = zMed;
					}
					done = true;
				}
				else 
				{
					kernelSize += 2;
				}
			}
			if (!done) 
			{
				result.at<uchar>(y, x) = src.at<uchar>(y, x);
			}
		}
	}
	dst->GetImage() = result;
}

void MotionBlur(Image* image, Image* dst, int size, double angle)
{
	auto kernel = GetMotionBlurKernel(size, angle);
	cv::Mat blurred;
	cv::filter2D(image->GetImage(), blurred, -1, kernel);
	dst->GetImage() = blurred;
}

cv::Mat GetMotionBlurKernel(int size, double angle)
{
	cv::Mat kernel = cv::Mat::zeros(size, size, CV_32F);
	int center = size / 2;

	for (int i = 0; i < size; i++) 
	{
		int x = static_cast<int>(center + (i - center) * cos(angle));
		int y = static_cast<int>(center + (i - center) * sin(angle));
		if (x >= 0 && x < size && y >= 0 && y < size) 
		{
			kernel.at<float>(y, x) = 1.0f;
		}
	}

	return kernel / cv::sum(kernel)[0];
}

void WienerFilter(Image* image, Image* dst, const cv::Mat& kernel, float K)
{
	auto degraded = image->GetImage().clone();
	cv::Mat padded;
	int m = cv::getOptimalDFTSize(degraded.rows);
	int n = cv::getOptimalDFTSize(degraded.cols);
	cv::copyMakeBorder(degraded, padded, 0, m - degraded.rows, 0, n - degraded.cols,
	                   cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
	cv::Mat complexI;
	cv::merge(planes, 2, complexI);
	cv::dft(complexI, complexI);

	cv::Mat kernelPadded;
	cv::copyMakeBorder(kernel, kernelPadded, 0, m - kernel.rows, 0, n - kernel.cols,
	                   cv::BORDER_CONSTANT, cv::Scalar::all(0));
	cv::Mat planesH[] = { cv::Mat_<float>(kernelPadded), cv::Mat::zeros(kernelPadded.size(), CV_32F) };
	cv::Mat complexH;
	cv::merge(planesH, 2, complexH);
	cv::dft(complexH, complexH);

	cv::Mat complexDiv;
	cv::Mat planesDiv[] = { cv::Mat::zeros(complexI.size(), CV_32F),
		cv::Mat::zeros(complexI.size(), CV_32F) };

	for (int i = 0; i < complexI.rows; i++) 
	{
		for (int j = 0; j < complexI.cols; j++) 
		{
			cv::Vec2f H = complexH.at<cv::Vec2f>(i, j);
			cv::Vec2f G = complexI.at<cv::Vec2f>(i, j);
			float H2 = H[0] * H[0] + H[1] * H[1];
			float denominator = H2 + K;

			if (denominator != 0) {
				planesDiv[0].at<float>(i, j) = (H[0] * G[0] + H[1] * G[1]) / denominator;
				planesDiv[1].at<float>(i, j) = (H[0] * G[1] - H[1] * G[0]) / denominator;
			}
		}
	}

	cv::merge(planesDiv, 2, complexDiv);
	cv::idft(complexDiv, complexDiv);
	cv::split(complexDiv, planes);

	cv::normalize(planes[0], planes[0], 0, 255, cv::NORM_MINMAX);

	cv::Mat restored;
	planes[0](cv::Rect(0, 0, degraded.cols, degraded.rows)).copyTo(restored);
	restored.convertTo(restored, CV_8U);
	dst->GetImage() = restored;
}

cv::Mat RotateImage(const cv::Mat& src, float angle)
{
	cv::Point2f center(src.cols / 2.0F, src.rows / 2.0F);
	cv::Mat rotMatrix = cv::getRotationMatrix2D(center, angle, 1.0);
	cv::Mat dst;
	cv::warpAffine(src, dst, rotMatrix, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0));
	return dst;
}

cv::Mat RadonTransform(const cv::Mat& img, int angles)
{
	cv::Mat sinogram = cv::Mat::zeros(angles, img.cols, CV_32F);

	for (int i = 0; i < angles; i++)
	{
		float angle = i * 180.0 / angles;
		cv::Mat rotated = RotateImage(img, angle);

		for (int col = 0; col < rotated.cols; col++) 
		{
			sinogram.at<float>(i, col) = cv::sum(rotated.col(col))[0];
		}
	}
	cv::normalize(sinogram, sinogram, 0, 255, cv::NORM_MINMAX);
	sinogram.convertTo(sinogram, CV_8U);
	return sinogram;
}

cv::Mat FilterSinogram(const cv::Mat& sinogram)
{
	cv::Mat filtered;
	sinogram.convertTo(filtered, CV_32F);
	cv::Mat kernel = (cv::Mat_<float>(1, 3) << -1, 2, -1);
	cv::filter2D(filtered, filtered, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);
	return filtered;
}

cv::Mat BackProjection(const cv::Mat& filteredSinogram, int imgSize)
{
	int angles = filteredSinogram.rows;
	cv::Mat reconstructed = cv::Mat::zeros(imgSize, imgSize, CV_32F);

	for (int i = 0; i < angles; i++) 
	{
		float angle = i * 180.0 / angles;
		cv::Mat projection = filteredSinogram.row(i).clone();

		cv::Mat backProj = cv::Mat::zeros(imgSize, imgSize, CV_32F);
		for (int col = 0; col < backProj.cols; col++) 
		{
			backProj.col(col) += projection.at<float>(col);
		}

		backProj = RotateImage(backProj, -angle);
		reconstructed += backProj;
	}

	cv::normalize(reconstructed, reconstructed, 0, 255, cv::NORM_MINMAX);
	reconstructed.convertTo(reconstructed, CV_8U);
	return reconstructed;
}
