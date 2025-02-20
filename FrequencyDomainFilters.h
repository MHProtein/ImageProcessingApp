#pragma once
#ifndef FREQUENCY_DOMAIN_FILTERS_H
#define FREQUENCY_DOMAIN_FILTERS_H
#include <iso646.h>
#include <opencv2/opencv.hpp>

#include "Image.h"

class Image;

cv::Mat ApplyFilter(cv::Mat& complexI, const cv::Mat& filter, int alpha = 0, int beta = 1);

cv::Mat ComputePhaseSpectrum(const cv::Mat& complexI);

void LowPassFilter(Image* image, Image* dest, Image* frequencyDest, double d0, int type, int n = 2);

void HighPassFilter(Image* image, Image* dest, Image* frequencyDest, double d0, int type, int n = 2);

cv::Mat CreateHomomorphicFilter(cv::Size size, float d0, float gammaL, float gammaH, float c);

void ApplyHomomorphicFilter(Image* image, Image* dest, Image* frequencyDest, float d0, float gammaL, float gammaH, float c);

cv::Mat CreateNotchRejectFilter(cv::Size size, int u_k, int v_k, int d0);

void ApplyNotchRejectFilter(Image* image, Image* dest, Image* frequencyDest, int u_k, int v_k, int d0);

#endif // FREQUENCY_DOMAIN_FILTERS_H
