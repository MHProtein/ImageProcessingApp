#include "Opeartions.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <opencv2/opencv.hpp>

#include "DenoiseFilters.h"
#include "FloatSlider.h"
#include "FrequencyDomainFilters.h"
#include "SpaceFilters.h"
#include "ToolBarManager.h"

FloatValue::FloatValue(const QString& name, float minValue, float maxValue, float initialValue, QFormLayout* layout,
	const std::function<void()>& callback)
{
	value = initialValue;
	slider = new FloatSlider(minValue, maxValue, initialValue, nullptr, [&](float value)
		{
			SetValue(value);
			if (valueChangedGreaterThanThreshold)
			{
				if (callback)
					callback();
			}
		});
	QLabel* cLabel = new QLabel(name);
	layout->addRow(cLabel, slider);
}

void FloatValue::SetValue(float newValue)
{
	if (std::abs(newValue - value) > 0.1f)
	{
		valueChangedGreaterThanThreshold = true;
		value = newValue;
	}
	else
	{
		valueChangedGreaterThanThreshold = false;
	}
}

bool FloatValue::operator<(float other) const
{
	return value < other;
}

bool FloatValue::operator==(float other) const
{
	return value == other;
}

IntValue::IntValue(const QString& name, int minValue, int maxValue, int initialValue, bool oddOnly, QFormLayout* layout,
	const std::function<void()>& callback)
{
	value = initialValue;
	slider = new IntSlider(minValue, maxValue, initialValue, oddOnly, name, nullptr, [&](int value)
		{
			SetValue(value);
			if (callback)
				callback();
		});
	QLabel* cLabel = new QLabel(name);
	layout->addRow(cLabel, slider);
}

void IntValue::SetValue(int newValue)
{
	value = newValue;
}

bool IntValue::operator<(int other) const
{
	return value < other;
}

bool IntValue::operator==(int other) const
{
	return value == other;
}

void AdditionOperation::OnExecute()
{
	if (lhsImage->GetSize() != rhsImage->GetSize())
		return;
	lhsImage->Add(rhsImage, dest);
}


void SubtractionOperation::OnExecute()
{
	if (lhsImage->GetSize() != rhsImage->GetSize())
		return;
	lhsImage->Subtract(rhsImage, dest);
}

void MultiplicationOperation::OnExecute()
{
	if (lhsImage->GetSize() != rhsImage->GetSize())
		return;
	lhsImage->Multiply(rhsImage, dest);
}

void DivisionOperation::OnExecute()
{
	if (lhsImage->GetSize() != rhsImage->GetSize())
		return;
	lhsImage->Divide(rhsImage, dest);
}

ExponentialOperation::ExponentialOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	gamma = new FloatValue("Gamma", 0.1f, 10.0f, 1.0f, formLayout, UpdatePreview);
}

void ExponentialOperation::OnExecute()
{
	srcImage->GetGammaTransformation(gamma->Value(), dest);
}

LogarithmOperation::LogarithmOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{

}

void LogarithmOperation::OnExecute()
{
	srcImage->GetLogTransformation(dest);
}

HistogramEqualizationOperation::HistogramEqualizationOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
}

void HistogramEqualizationOperation::OnExecute()
{
	srcImage->GetHistogramEqualization(dest);
}

HistogramSpecificationOperation::HistogramSpecificationOperation(QWidget* parent) : BinaryOperandImageOperation(parent)
{
}

void HistogramSpecificationOperation::OnExecute()
{
	if (lhsImage->GetSize() != rhsImage->GetSize())
		return;
	lhsImage->GetHistogramSpecification(rhsImage, dest);
}

BoxFilterOperation::BoxFilterOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, false, formLayout, UpdatePreview);
}

void BoxFilterOperation::OnExecute()
{
	BoxFilter(srcImage, dest, cv::Size(size->Value(), size->Value()));
}

GaussianBlurOperation::GaussianBlurOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, true, formLayout, UpdatePreview);
	sigma = new FloatValue("Sigma", 0.1f, 10.0f, 1.0f, formLayout, UpdatePreview);
}

void GaussianBlurOperation::OnExecute()
{
	qDebug() << sigma->Value();
	GaussianBlur(srcImage, dest, cv::Size(size->Value(), size->Value()), sigma->Value());
}

MinFilterOperation::MinFilterOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, true, formLayout, UpdatePreview);
}

void MinFilterOperation::OnExecute()
{
	MinFilter(srcImage, dest, cv::Size(size->Value(), size->Value()));
}

MedianFilterOperation::MedianFilterOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, true, formLayout, UpdatePreview);
}

void MedianFilterOperation::OnExecute()
{
	MedianFilter(srcImage, dest, cv::Size(size->Value(), size->Value()));
}

MaxFilterOperation::MaxFilterOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, true, formLayout, UpdatePreview);
}

void MaxFilterOperation::OnExecute()
{
	MaxFilter(srcImage, dest, cv::Size(size->Value(), size->Value()));
}

Laplacian8Operation::Laplacian8Operation(QWidget* parent) : SingleOperandImageOperation(parent)
{

}

void Laplacian8Operation::OnExecute()
{
	Laplacian8(srcImage, dest);
}

Laplacian4Operation::Laplacian4Operation(QWidget* parent) : SingleOperandImageOperation(parent)
{

}

void Laplacian4Operation::OnExecute()
{
	Laplacian4(srcImage, dest);
}

SobelOperation::SobelOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	dx = new IntValue("Size", 0, 1, 0, false, formLayout, UpdatePreview);
	dy = new IntValue("Size", 0, 1, 0, false, formLayout, UpdatePreview);
}

void SobelOperation::OnExecute()
{
	if (dx->Value() == 0 && dy->Value() == 0)
	{
		dest->GetImage() = srcImage->GetImage();
		return;
	}
	qDebug() << dx->Value() << " " << dy->Value();
	Sobel(srcImage, dest, dx->Value(), dy->Value());
}

RobertsOperation::RobertsOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{

}

void RobertsOperation::OnExecute()
{
	Roberts(srcImage, dest);
}

LowPassFilterOperation::LowPassFilterOperation(QWidget* parent) : FrequencyDomainOperation(parent)
{
	type = new QComboBox();
	type->addItem("Ideal");
	type->addItem("Gaussian");
	type->addItem("Butterworth");
	formLayout->addRow("Type", type);

	d0 = new FloatValue("D0", 0.0f, 5000.0f, 500.0f, formLayout, FrequencyUpdatePreview);
	n = new FloatValue("N(Butterworth)", 1.0f, 10.0f, 2.0f, formLayout, ButterWorthUpdatePreview);
}

void LowPassFilterOperation::OnExecute()
{
	LowPassFilter(srcImage, dest, frequencyDest, d0->Value(), type->currentIndex(), n->Value());
}

HighPassFilterOperation::HighPassFilterOperation(QWidget* parent) : FrequencyDomainOperation(parent)
{
	type = new QComboBox();
	type->addItem("Ideal");
	type->addItem("Gaussian");
	type->addItem("Butterworth");
	formLayout->addRow("Type", type);

	d0 = new FloatValue("D0", 0.0f, 5000.0f, 500.0f, formLayout, FrequencyUpdatePreview);
	n = new FloatValue("N(Butterworth)", 1.0f, 10.0f, 2.0f, formLayout, ButterWorthUpdatePreview);
}

void HighPassFilterOperation::OnExecute()
{
	HighPassFilter(srcImage, dest, frequencyDest, d0->Value(), type->currentIndex(), n->Value());
}

HomomorphicFilterOperation::HomomorphicFilterOperation(QWidget* parent) : FrequencyDomainOperation(parent)
{
	d0 = new FloatValue("D0", 0.0f, 5000.0f, 500.0f, formLayout, FrequencyUpdatePreview);
	gammaL = new FloatValue("GammaL", 0.1f, 10.0f, 1.0f, formLayout, FrequencyUpdatePreview);
	gammaH = new FloatValue("GammaH", 0.1f, 10.0f, 1.0f, formLayout, FrequencyUpdatePreview);
	c = new FloatValue("C", 0.1f, 10.0f, 1.0f, formLayout, FrequencyUpdatePreview);
}

void HomomorphicFilterOperation::OnExecute()
{
	ApplyHomomorphicFilter(srcImage, dest, frequencyDest, d0->Value(), gammaL->Value(), gammaH->Value(), c->Value());
}

NotchRejectFilterOperation::NotchRejectFilterOperation(QWidget* parent) : FrequencyDomainOperation(parent)
{
	u_k = new FloatValue("U_k", 0, 5000.0f, 0.0f, formLayout, FrequencyUpdatePreview);
	v_k = new FloatValue("V_k", 0, 5000.0f, 0.0f, formLayout, FrequencyUpdatePreview);
	d0 = new FloatValue("D0", 0.1f, 5000.0f, 500.0f, formLayout, FrequencyUpdatePreview);
}

void NotchRejectFilterOperation::OnExecute()
{
	ApplyNotchRejectFilter(srcImage, dest, frequencyDest, u_k->Value(), v_k->Value(), d0->Value());
}

GaussianNoiseOperation::GaussianNoiseOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	mean = new FloatValue("Mean", 0.0f, 100.0f, 0.0f, formLayout, UpdatePreview);
	var = new FloatValue("Var", 0.0f, 100.0f, 0.0f, formLayout, UpdatePreview);
}

void GaussianNoiseOperation::OnExecute()
{
	AddGaussianNoise(srcImage, dest, mean->Value(), var->Value());
}

SaltAndPepperNoiseOperation::SaltAndPepperNoiseOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	saltProb = new FloatValue("Salt Prob", 0.0f, 1.0f, 0.1f, formLayout, UpdatePreview);
	pepperProb = new FloatValue("Pepper Prob", 0.0f, 1.0f, 0.1f, formLayout, UpdatePreview);
}

void SaltAndPepperNoiseOperation::OnExecute()
{
	AddSaltAndPepperNoise(srcImage, dest, saltProb->Value(), pepperProb->Value());
}

ArithmeticMeanFilterOperation::ArithmeticMeanFilterOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, true, formLayout, UpdatePreview);
}

void ArithmeticMeanFilterOperation::OnExecute()
{
	ArithmeticMeanFilter(srcImage, dest, size->Value());
}

GeometricMeanFilterOperation::GeometricMeanFilterOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, true, formLayout, UpdatePreview);
}

void GeometricMeanFilterOperation::OnExecute()
{
	GeometricMeanFilter(srcImage, dest, size->Value());
}

AdaptiveLocalNoiseReductionOperation::AdaptiveLocalNoiseReductionOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, true, formLayout, UpdatePreview);
	noiseVariance = new FloatValue("NoiseVariance", 0, 100, 1, formLayout, UpdatePreview);
}

void AdaptiveLocalNoiseReductionOperation::OnExecute()
{
	AdaptiveLocalNoiseReductionFilter(srcImage, dest, size->Value(), noiseVariance->Value());
}

MedianDenoiseFilterOperation::MedianDenoiseFilterOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, true, formLayout, UpdatePreview);
}

void MedianDenoiseFilterOperation::OnExecute()
{
	MedianFilter(srcImage, dest, size->Value());
}

AlphaTrimmedMeanFilterOperation::AlphaTrimmedMeanFilterOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	size = new IntValue("Size", 1, 25, 5, true, formLayout, UpdatePreview);
	d = new IntValue("d", 2, 100, 6, false, formLayout, UpdatePreview);
}

void AlphaTrimmedMeanFilterOperation::OnExecute()
{
	AlphaTrimmedMeanFilter(srcImage, dest, size->Value(), d->Value());
}

AdaptiveMedianFilterOperation::AdaptiveMedianFilterOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	maxSize = new IntValue("MaxSize", 1, 25, 5, true, formLayout, UpdatePreview);
}

void AdaptiveMedianFilterOperation::OnExecute()
{
	AdaptiveMedianFilter(srcImage, dest, maxSize->Value());
}

MotionBlurOperation::MotionBlurOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
}

void MotionBlurOperation::OnExecute()
{
	MotionBlur(srcImage, dest, 30, 45);
}

InverseFilteringOperation::InverseFilteringOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	kernel = GetMotionBlurKernel(30, 45);
}

void InverseFilteringOperation::OnExecute()
{
	WienerFilter(srcImage, dest, kernel, 0.0001f);

}

LowPassInverseOperation::LowPassInverseOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	kernel = GetMotionBlurKernel(30, 45);
}

void LowPassInverseOperation::OnExecute()
{
	Image* lowPassDegraded = new Image;
	cv::GaussianBlur(srcImage->GetImage(), lowPassDegraded->GetImage(), cv::Size(3, 3), 1.0);
	WienerFilter(lowPassDegraded, dest, kernel, 0.0001f);
	
}

WienerDeconvolutionOperation::WienerDeconvolutionOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
	kernel = GetMotionBlurKernel(30, 45);
	K = new FloatValue("K", 0.0f, 1.0f, 0.0001f, formLayout, UpdatePreview);
}

void WienerDeconvolutionOperation::OnExecute()
{
	WienerFilter(srcImage, dest, kernel, K->Value());
	
}

RadonTransformOperation::RadonTransformOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
}

void RadonTransformOperation::OnExecute()
{
	int numAngles = 180;
	cv::Mat sinogram = RadonTransform(srcImage->GetImage(), numAngles);

	cv::Mat filteredSinogram = FilterSinogram(sinogram);

	int reconstructedSize = std::max(srcImage->GetWidth(), srcImage->GetHeight());
	cv::Mat reconstructedImg = BackProjection(filteredSinogram, reconstructedSize);
	reconstructedImg.convertTo(reconstructedImg, CV_8U);
	dest->GetImage() = reconstructedImg;
}
