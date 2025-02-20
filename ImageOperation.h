#pragma once
#ifndef IMAGE_OPERATION_H
#define IMAGE_OPERATION_H
#include <QObject>

#include "ImageLoader.h"
#include "MatUtils.h"
#include "ToolBarManager.h"

class QMimeData;
class QComboBox;
class QVBoxLayout;
class ImageListItemContainerWidget;
class QPushButton;
class QFormLayout;

class ImageOperation : public QObject
{
	Q_OBJECT
public:
	ImageOperation(QWidget* parent);
	void Execute();

protected:
	virtual void OnExecute() = 0;
	virtual void DoExecution() = 0;
	Image* CreateNewImage(Image* ref);

	QVBoxLayout* mainLayout;
	QFormLayout* formLayout;
	QPushButton* executionButton;
	QWidget* parent;
	Image* dest;
};

class SingleOperandImageOperation : public ImageOperation
{
public:
	enum DestType
	{
		NEW_IMAGE = 0,
		SRC = 1
	};

	SingleOperandImageOperation(QWidget* parent);
	virtual void ReadSrc(QMimeData* data);
protected:

	ImageListItemContainerWidget* src;
	Image* srcImage;
	int index;
	QComboBox* destType;
	std::function<void()> UpdatePreview = [&]()
		{
			if (!srcImage)
				return;
			dest = ToolBarManager::GetInstance()->GetPreview();
			OnExecute();
			ToolBarManager::GetInstance()->UpdatePreview();
		};

private:
	void DoExecution() override;
};


class BinaryOperandImageOperation : public ImageOperation
{
public:
	enum DestType
	{
		NEW_IMAGE = 0,
		LHS = 1,
		RHS = 2
	};

	BinaryOperandImageOperation(QWidget* parent);

	virtual void ReadLHS(QMimeData* data);
	virtual void ReadRHS(QMimeData* data);

protected:
	ImageListItemContainerWidget* lhs;
	ImageListItemContainerWidget* rhs;
	Image* lhsImage;
	Image* rhsImage;
	int lhsIndex;
	int rhsIndex;
	QComboBox* destType;

	std::function<void()> UpdatePreview = [&]()
		{
			if (!lhsImage || !rhsImage)
				return;
			dest = ToolBarManager::GetInstance()->GetPreview();
			OnExecute();
			ToolBarManager::GetInstance()->UpdatePreview();
		};

private:
	void DoExecution() override;
};

class FrequencyDomainOperation : public SingleOperandImageOperation
{
public:
	FrequencyDomainOperation(QWidget* parent);
	void ReadSrc(QMimeData* data) override;

protected:
	Image* frequencyDest;

	std::function<void()> FrequencyUpdatePreview = [&]()
		{
			if (!srcImage)
				return;
			dest = ToolBarManager::GetInstance()->GetPreview();
			frequencyDest = ToolBarManager::GetInstance()->GetFrequencyPreview();
			OnExecute();
			ToolBarManager::GetInstance()->UpdatePreview();
			ToolBarManager::GetInstance()->UpdateFrequencyPreview();
		};
};

#endif // IMAGE_OPERATION_H