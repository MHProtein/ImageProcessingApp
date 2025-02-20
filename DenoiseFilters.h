#pragma once
#include <opencv2/opencv.hpp>

#include "Image.h"

void AddGaussianNoise(Image* image, Image* dst, float mean, float var);

void ArithmeticMeanFilter(Image* image, Image* dst, int kernelSize);

void GeometricMeanFilter(Image* image, Image* dst, int kernelSize);

void AdaptiveLocalNoiseReductionFilter(Image* image, Image* dst, int kernelSize, float noiseVariance);

void AddSaltAndPepperNoise(Image* image, Image* dst, float saltProb, float pepperProb);

void MedianFilter(Image* image, Image* dst, int kernelSize);

void AlphaTrimmedMeanFilter(Image* image, Image* dst, int kernelSize, int d);

void AdaptiveMedianFilter(Image* image, Image* dst, int maxKernelSize);

void MotionBlur(Image* image, Image* dst, int size, double angle);
cv::Mat GetMotionBlurKernel(int size, double angle);

void WienerFilter(Image* image, Image* dst, const cv::Mat& kernel, float K);

cv::Mat RotateImage(const cv::Mat& src, float angle);

cv::Mat RadonTransform(const cv::Mat& img, int angles);

cv::Mat FilterSinogram(const cv::Mat& sinogram);

cv::Mat BackProjection(const cv::Mat& filteredSinogram, int imgSize);
