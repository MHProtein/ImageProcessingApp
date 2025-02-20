#pragma once
#ifndef SPACE_FILTERS_H
#define SPACE_FILTERS_H
#include "Image.h"

void BoxFilter(Image* src, Image* dest, cv::Size size);

void GaussianBlur(Image* src, Image* dest, cv::Size size, float sigma);

void MinFilter(Image* src, Image* dest, cv::Size size);

void MedianFilter(Image* src, Image* dest, cv::Size size);

void MaxFilter(Image* src, Image* dest, cv::Size size);

void Laplacian8(Image* src, Image* dest);

void Laplacian4(Image* src, Image* dest);

void Sobel(Image* src, Image* dest, int dx, int dy);

void Roberts(Image* src, Image* dest);

#endif // SPACE_FILTERS_H
