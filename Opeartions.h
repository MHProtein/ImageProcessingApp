#pragma once

#include <functional>
#include <QComboBox>

#include "Image.h"
#include "ImageOperation.h"

class IntSlider;
class FloatSlider;
class QWidget;

class FloatValue
{
public:
    bool valueChangedGreaterThanThreshold = false;

    FloatValue(const QString& name, float minValue, float maxValue, float initialValue, QFormLayout* layout,
		const std::function<void()>& callback);

    float Value() const { return value; }

    void SetValue(float newValue);

    bool operator<(float other) const;

    bool operator==(float other) const;

private:
    float value;
    FloatSlider* slider;
};

class IntValue
{
public:
	IntValue(const QString& name, int minValue, int maxValue, int initialValue, bool oddOnly, QFormLayout* layout,
		const std::function<void()>& callback);

	int Value() const { return value; }

	void SetValue(int newValue);

	bool operator<(int other) const;

	bool operator==(int other) const;

private:
	int value;
	IntSlider* slider;
};

class AdditionOperation : public BinaryOperandImageOperation
{
public:
	AdditionOperation(QWidget* parent) : BinaryOperandImageOperation(parent) {}
	void OnExecute() override;
};


class SubtractionOperation : public BinaryOperandImageOperation
{
public:
	SubtractionOperation(QWidget* parent) : BinaryOperandImageOperation(parent) {}
	void OnExecute() override;
};

class MultiplicationOperation : public BinaryOperandImageOperation
{
public:
	MultiplicationOperation(QWidget* parent) : BinaryOperandImageOperation(parent) {}
	void OnExecute() override;
};

class DivisionOperation : public BinaryOperandImageOperation
{
public:
	DivisionOperation(QWidget* parent) : BinaryOperandImageOperation(parent) {}
	void OnExecute() override;
};

class ExponentialOperation : public SingleOperandImageOperation
{
public:
	ExponentialOperation(QWidget* parent);
	void OnExecute() override;
private:
	FloatValue* gamma;
};

class LogarithmOperation : public SingleOperandImageOperation
{
public:
	LogarithmOperation(QWidget* parent);
	void OnExecute() override;
};

class HistogramEqualizationOperation : public SingleOperandImageOperation
{
public:
	HistogramEqualizationOperation(QWidget* parent);
	void OnExecute() override;
};

class HistogramSpecificationOperation : public BinaryOperandImageOperation
{
public:
	HistogramSpecificationOperation(QWidget* parent);
	void OnExecute() override;
};

class BoxFilterOperation : public SingleOperandImageOperation
{
public:
	BoxFilterOperation(QWidget* parent);

	void OnExecute() override;
private:

	IntValue* size;
};

class GaussianBlurOperation : public SingleOperandImageOperation
{
public:
	GaussianBlurOperation(QWidget* parent);

	void OnExecute() override;

private:
	IntValue* size;
	FloatValue* sigma;
};

class MinFilterOperation : public SingleOperandImageOperation
{
public:
	MinFilterOperation(QWidget* parent);

	void OnExecute() override;
private:

	IntValue* size;
};

class MedianFilterOperation : public SingleOperandImageOperation
{
public:
	MedianFilterOperation(QWidget* parent);

	void OnExecute() override;
private:

	IntValue* size;
};

class MaxFilterOperation : public SingleOperandImageOperation
{
public:
	MaxFilterOperation(QWidget* parent);

	void OnExecute() override;
private:

	IntValue* size;
};

class Laplacian8Operation : public SingleOperandImageOperation
{
public:
	Laplacian8Operation(QWidget* parent);

	void OnExecute() override;
};

class Laplacian4Operation : public SingleOperandImageOperation
{
public:
	Laplacian4Operation(QWidget* parent);

	void OnExecute() override;
};

class SobelOperation : public SingleOperandImageOperation
{
public:
	SobelOperation(QWidget* parent);

	void OnExecute() override;

private:
	IntValue* dx;
	IntValue* dy;
};

class RobertsOperation : public SingleOperandImageOperation
{
public:
	RobertsOperation(QWidget* parent);

	void OnExecute() override;
};


class LowPassFilterOperation : public FrequencyDomainOperation
{
public:
	LowPassFilterOperation(QWidget* parent);

	void OnExecute() override;
private:
	enum LowPassFilterType
	{
		IDEAL = 0,
		GAUSSIAN = 1,
		BUTTERWORTH = 2
	};

	QComboBox* type;
	FloatValue* d0;
	FloatValue* n;

	std::function<void()> ButterWorthUpdatePreview = [&]()
	{
		if (type->currentIndex() == BUTTERWORTH)
			FrequencyUpdatePreview();
	};
};

class HighPassFilterOperation : public FrequencyDomainOperation
{
public:
	HighPassFilterOperation(QWidget* parent);

	void OnExecute() override;
private:
	enum LowPassFilterType
	{
		IDEAL = 0,
		GAUSSIAN = 1,
		BUTTERWORTH = 2
	};

	QComboBox* type;
	FloatValue* d0;
	FloatValue* n;

	std::function<void()> ButterWorthUpdatePreview = [&]()
		{
			if (type->currentIndex() == BUTTERWORTH)
				FrequencyUpdatePreview();
		};
};

class HomomorphicFilterOperation : public FrequencyDomainOperation
{
public:
	HomomorphicFilterOperation(QWidget* parent);

	void OnExecute() override;
private:
	FloatValue* d0;
	FloatValue* gammaL;
	FloatValue* gammaH;
	FloatValue* c;
};

class NotchRejectFilterOperation : public FrequencyDomainOperation
{
public:
	NotchRejectFilterOperation(QWidget* parent);

	void OnExecute() override;
private:
	FloatValue* d0;
	FloatValue* u_k;
	FloatValue* v_k;
};






class GaussianNoiseOperation : public SingleOperandImageOperation
{
public:
	GaussianNoiseOperation(QWidget* parent);

	void OnExecute() override;
private:
	FloatValue* mean;
	FloatValue* var;
};

class SaltAndPepperNoiseOperation : public SingleOperandImageOperation
{
public:
	SaltAndPepperNoiseOperation(QWidget* parent);

	void OnExecute() override;
private:
	FloatValue* saltProb;
	FloatValue* pepperProb;
};

class ArithmeticMeanFilterOperation : public SingleOperandImageOperation
{
public:
	ArithmeticMeanFilterOperation(QWidget* parent);

	void OnExecute() override;
private:
	IntValue* size;
};

class GeometricMeanFilterOperation : public SingleOperandImageOperation
{
public:
	GeometricMeanFilterOperation(QWidget* parent);

	void OnExecute() override;
private:
	IntValue* size;
};
class AdaptiveLocalNoiseReductionOperation : public SingleOperandImageOperation
{
public:
	AdaptiveLocalNoiseReductionOperation(QWidget* parent);

	void OnExecute() override;
private:
	IntValue* size;
	FloatValue* noiseVariance;
};


class MedianDenoiseFilterOperation : public SingleOperandImageOperation
{
public:
	MedianDenoiseFilterOperation(QWidget* parent);

	void OnExecute() override;
private:
	IntValue* size;
};

class AlphaTrimmedMeanFilterOperation : public SingleOperandImageOperation
{
public:
	AlphaTrimmedMeanFilterOperation(QWidget* parent);

	void OnExecute() override;
private:
	IntValue* size;
	IntValue* d;
};

class AdaptiveMedianFilterOperation : public SingleOperandImageOperation
{
public:
	AdaptiveMedianFilterOperation(QWidget* parent);

	void OnExecute() override;
private:
	IntValue* maxSize;
};

class MotionBlurOperation : public SingleOperandImageOperation
{
public:
	MotionBlurOperation(QWidget* parent);

	void OnExecute() override;
};

class InverseFilteringOperation : public SingleOperandImageOperation
{
public:
	InverseFilteringOperation(QWidget* parent);

	void OnExecute() override;

private:
	cv::Mat kernel;
};

class LowPassInverseOperation : public SingleOperandImageOperation
{
public:
	LowPassInverseOperation(QWidget* parent);

	void OnExecute() override;
private:
	cv::Mat kernel;
};

class WienerDeconvolutionOperation : public SingleOperandImageOperation
{
public:
	WienerDeconvolutionOperation(QWidget* parent);

	void OnExecute() override;

private:
	FloatValue* K;
	cv::Mat kernel;
};

class RadonTransformOperation : public SingleOperandImageOperation
{
public:
	RadonTransformOperation(QWidget* parent);

	void OnExecute() override;

private:
	IntValue* K;
};