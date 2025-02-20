#pragma once
#include <QListWidget>

#include "ImageLoader.h"
#include "Singleton.h"

class ImageListWidget;

class ToolBarManager : public Singleton<ToolBarManager>
{
public:
	void Initialization(Ui::MainWindow* ui);
	void AddItem(int index, QListWidgetItem* item);
	void RemoveItem(int index);
	void RemoveItem(QListWidgetItem* item);
	int GetIndex(QListWidgetItem* tab);
	QListWidgetItem* GetItem(int index);

	Image* GetPreview();
	void UpdatePreview() const;
	Image* GetFrequencyPreview();
	void UpdateFrequencyPreview() const;
private:

	void OnItemClicked(QListWidgetItem* item);
	void OnItemDoubleClicked(QListWidgetItem* item);

	std::unordered_map<int, QListWidgetItem*> items;

	ImageListWidget* list;

	class AdditionOperation* additionOp;
	class SubtractionOperation* subtractionOp;
	class MultiplicationOperation* multiplicationOp;
	class DivisionOperation* divisionOp;
	class ExponentialOperation* exponentialOp;
	class LogarithmOperation* logarithmOp;
	class HistogramEqualizationOperation* histogramEqualizationOp;
	class HistogramSpecificationOperation* histogramSpecificationOp;

	class BoxFilterOperation* boxFilterOp;
	class GaussianBlurOperation* gaussianBlurOp;
	class MinFilterOperation* minFilterOp;
	class MedianFilterOperation* medianFilterOp;
	class MaxFilterOperation* maxFilterOp;
	class Laplacian8Operation* laplacian8Op;
	class Laplacian4Operation* laplacian4Op;
	class SobelOperation* sobelOp;
	class RobertsOperation* robertsOp;

	class LowPassFilterOperation* lowPassFilterOp;
	class HighPassFilterOperation* highPassFilterOp;
	class HomomorphicFilterOperation* homomorphicFilterOp;
	class NotchRejectFilterOperation* notchRejectFilterOp;

	class GaussianNoiseOperation* gaussianNoiseOp;
	class SaltAndPepperNoiseOperation* saltAndPepperNoiseOp;
	class MotionBlurOperation* motionBlurOp;
	class ArithmeticMeanFilterOperation* arithmeticMeanFilterOp;
	class GeometricMeanFilterOperation* geometricMeanFilterOp;
	class AdaptiveLocalNoiseReductionOperation* adaptiveLocalNoiseReductionOp;
	class MedianDenoiseFilterOperation* medianDenoiseFilterOp;
	class AlphaTrimmedMeanFilterOperation* alphaTrimmedMeanFilterOp;
	class AdaptiveMedianFilterOperation* adaptiveMedianFilterOp;
	class InverseFilteringOperation* directDeconvolutionOp;
	class LowPassInverseOperation* lowPassDeconvolutionOp;
	class WienerDeconvolutionOperation* wienerDeconvolutionOp;
	class RadonTransformOperation* radonTransformOp;

	Image* preview;
	Image* frequencyPreview;
	bool previewTabSet;
};

