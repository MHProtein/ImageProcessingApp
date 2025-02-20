#include "ToolBarManager.h"

#include <ui_mainwindow.h>

#include "ImageListWidget.h"
#include "ImageTab.h"
#include "ImageTabManager.h"
#include "Opeartions.h"

void ToolBarManager::Initialization(Ui::MainWindow* ui)
{
	this->list = ui->ImagesList;
	additionOp = new AdditionOperation(ui->Add);
	subtractionOp = new SubtractionOperation(ui->Subtract);;
	multiplicationOp = new MultiplicationOperation(ui->Multiply);
	divisionOp = new DivisionOperation(ui->Division);
	exponentialOp = new ExponentialOperation(ui->Exponential);
	logarithmOp = new LogarithmOperation(ui->Logarithm);
	histogramEqualizationOp = new HistogramEqualizationOperation(ui->HistogramEqualization);
	histogramSpecificationOp = new HistogramSpecificationOperation(ui->HistogramSpecification);

	boxFilterOp = new BoxFilterOperation(ui->BoxFilter);
	gaussianBlurOp = new GaussianBlurOperation(ui->GaussianBlur);
	minFilterOp = new MinFilterOperation(ui->MinFilter);
	medianFilterOp = new MedianFilterOperation(ui->MedianFilter);
	maxFilterOp = new MaxFilterOperation(ui->MaxFilter);
	laplacian8Op = new Laplacian8Operation(ui->Laplacian8);
	laplacian4Op = new Laplacian4Operation(ui->Laplacian4);
	sobelOp = new SobelOperation(ui->Sobel);
	robertsOp = new RobertsOperation(ui->Roberts);

	lowPassFilterOp = new LowPassFilterOperation(ui->LowPassFilters);
	highPassFilterOp = new HighPassFilterOperation(ui->HighPassFilters);
	homomorphicFilterOp = new HomomorphicFilterOperation(ui->HomomorphicFilter);
	notchRejectFilterOp = new NotchRejectFilterOperation(ui->NotchRejection);

	gaussianNoiseOp = new GaussianNoiseOperation(ui->GaussianNoise);
	saltAndPepperNoiseOp = new SaltAndPepperNoiseOperation(ui->SaltAndPepperNoise);
	motionBlurOp = new MotionBlurOperation(ui->MotionBlur);
	arithmeticMeanFilterOp = new ArithmeticMeanFilterOperation(ui->ArithmeticMeanFilter);
	geometricMeanFilterOp = new GeometricMeanFilterOperation(ui->GeometricMeanFilter);
	adaptiveLocalNoiseReductionOp = new AdaptiveLocalNoiseReductionOperation(ui->AdaptiveLocalNoiseReductionFilter);
	medianDenoiseFilterOp = new MedianDenoiseFilterOperation(ui->MedianFilter1);
	alphaTrimmedMeanFilterOp = new AlphaTrimmedMeanFilterOperation(ui->AlphaTrimmedMeanFilter);
	adaptiveMedianFilterOp = new AdaptiveMedianFilterOperation(ui->AdaptiveMedianFilter);
	directDeconvolutionOp = new InverseFilteringOperation(ui->InverseFiltering);
	lowPassDeconvolutionOp = new LowPassInverseOperation(ui->LowPassInverseFiltering);
	wienerDeconvolutionOp = new WienerDeconvolutionOperation(ui->Wiener);
	radonTransformOp = new RadonTransformOperation(ui->RadonTransform);

	connect(list, &ImageListWidget::itemClicked, this, &ToolBarManager::OnItemClicked);
	connect(list, &ImageListWidget::itemDoubleClicked, this, &ToolBarManager::OnItemDoubleClicked);
}

void ToolBarManager::AddItem(int index, QListWidgetItem* item)
{
	item->setFlags(item->flags() | Qt::ItemIsSelectable);
	items.insert(std::make_pair(index, item));
	list->addItem(item);
}

void ToolBarManager::RemoveItem(int index)
{
	list->removeItemWidget(items[index]);
	items[index]->setHidden(true);
	items.erase(index);
}

void ToolBarManager::RemoveItem(QListWidgetItem* item)
{
	list->removeItemWidget(item);
	item->setHidden(true);
	for (auto it = items.begin(); it != items.end(); it++)
	{
		if (it->second == item)
		{
			items.erase(it);
			break;
		}
	}
}

int ToolBarManager::GetIndex(QListWidgetItem* tab)
{
	for (auto it = items.begin(); it != items.end(); it++)
	{
		if (it->second == tab)
		{
			return it->first;
		}
	}
	return -1;
}

QListWidgetItem* ToolBarManager::GetItem(int index)
{
	return items[index];
}

Image* ToolBarManager::GetPreview()
{
	if (preview)
		return preview;
	preview = ImageLoader::GetInstance()->CreateImage("Preview", 1500, 1500);
	return preview;
}

void ToolBarManager::UpdatePreview() const
{
	if (!preview)
		return;
	ImageTabManager::GetInstance()->GetTab(preview->GetIndex())->UpdateImage(preview);
	if ( (frequencyPreview && ImageTabManager::GetInstance()->isCurrentTab(frequencyPreview->GetIndex())) ||
		ImageTabManager::GetInstance()->isCurrentTab(preview->GetIndex()))
		return;
	ImageTabManager::GetInstance()->SetTabIndex(preview->GetIndex());
}

Image* ToolBarManager::GetFrequencyPreview()
{
	if (frequencyPreview)
		return frequencyPreview;
	frequencyPreview = ImageLoader::GetInstance()->CreateImage("FrequencyPreview", 1500, 1500);
	return frequencyPreview;
}

void ToolBarManager::UpdateFrequencyPreview() const
{
	if (!frequencyPreview)
		return;
	ImageTabManager::GetInstance()->GetTab(frequencyPreview->GetIndex())->UpdateImage(frequencyPreview);
	qDebug() << "UpdateFrequencyPreview";
	if (ImageTabManager::GetInstance()->isCurrentTab(frequencyPreview->GetIndex()) ||
		(preview && ImageTabManager::GetInstance()->isCurrentTab(preview->GetIndex())))
		return;
	ImageTabManager::GetInstance()->SetTabIndex(frequencyPreview->GetIndex());
}

void ToolBarManager::OnItemClicked(QListWidgetItem* item)
{
	auto index = GetIndex(item);
	ImageTabManager::GetInstance()->SwitchToTab(index);
}

void ToolBarManager::OnItemDoubleClicked(QListWidgetItem* item)
{
	auto index = GetIndex(item);
	ImageTabManager::GetInstance()->SwitchToTab(index);
}
