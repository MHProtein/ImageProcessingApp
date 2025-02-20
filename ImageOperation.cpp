#include "ImageOperation.h"

#include "ImageListItemContainerWidget.h"

#include <ui_mainwindow.h>
#include <QFormLayout>
#include <QComboBox>

#include "Image.h"
#include "ImageManager.h"
#include "ImageTab.h"
#include "ImageTabManager.h"
#include "NewImageDialog.h"
#include "ToolBarManager.h"

ImageOperation::ImageOperation(QWidget* parent) : parent(parent)
{
    mainLayout = new QVBoxLayout(parent);
    parent->setLayout(mainLayout);
    formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);
	mainLayout->setAlignment(Qt::AlignTop);
    executionButton = new QPushButton("Execute", parent);
    mainLayout->addWidget(executionButton);
	executionButton->setEnabled(false);
    connect(executionButton, &QPushButton::clicked, this, &ImageOperation::Execute);
}

void ImageOperation::Execute()
{
	DoExecution();
}

Image* ImageOperation::CreateNewImage(Image* ref)
{
	NewImageDialog* dialog = new NewImageDialog();
	QString name;
	if (dialog->exec() == QDialog::Accepted)
	{
		name = dialog->GetImageText();
	}
	else
	{
		delete dialog;
		return nullptr;
	}

	delete dialog;
	return ImageLoader::GetInstance()->CreateImage(name, ref->GetWidth(), ref->GetHeight());
}

SingleOperandImageOperation::SingleOperandImageOperation(QWidget* parent) : ImageOperation(parent), index(-1)
{
	QLabel* srcLabel = new QLabel("Src", parent);

	src = new ImageListItemContainerWidget(parent,[&](QMimeData* data) { ReadSrc(data); });
	formLayout->addRow(srcLabel, src);
	destType = new QComboBox(parent);
	destType->addItem("NEW_IMAGE");
	destType->addItem("SRC");
	formLayout->addRow(new QLabel("Dest"), destType);
}

void SingleOperandImageOperation::ReadSrc(QMimeData* data)
{
	QByteArray byteArray = data->data("application/x-extra-int");
	index = byteArray.toInt();
	srcImage = ImageManager::GetInstance()->GetImage(index);
	if(index != -1)
	{
		executionButton->setEnabled(true);
		UpdatePreview();
	}
}

void SingleOperandImageOperation::DoExecution()
{
	switch (destType->currentIndex())
	{
		case DestType::SRC:
			dest = srcImage;
			break;
		case DestType::NEW_IMAGE:
			dest = CreateNewImage(srcImage);
			break;
		default:
			return;
	}

	if (!dest)
		return;

	OnExecute();

	auto tab = ImageTabManager::GetInstance()->GetTab(dest->GetIndex());
	tab->UpdateImage(dest);
}

BinaryOperandImageOperation::BinaryOperandImageOperation(QWidget* parent) : ImageOperation(parent), lhsIndex(-1), rhsIndex(-1)
{
	QLabel* lhsLabel = new QLabel("LHS", parent);
	QLabel* rhsLabel = new QLabel("RHS", parent);

	lhs = new ImageListItemContainerWidget(parent, [&](QMimeData* data) { ReadLHS(data); });
	rhs = new ImageListItemContainerWidget(parent, [&](QMimeData* data) { ReadRHS(data); });

	formLayout->addRow(lhsLabel, lhs);
	formLayout->addRow(rhsLabel, rhs);

	destType = new QComboBox(parent);
	destType->addItem("NEW_IMAGE");
	destType->addItem("LHS");
	destType->addItem("RHS");
	formLayout->addRow(new QLabel("Dest"), destType);
}

void BinaryOperandImageOperation::ReadLHS(QMimeData* data)
{
	QByteArray byteArray = data->data("application/x-extra-int");
	lhsIndex = byteArray.toInt();

	lhsImage = ImageManager::GetInstance()->GetImage(lhsIndex);
	if(lhsIndex != -1 && rhsIndex != -1)
	{
		executionButton->setEnabled(true);
		UpdatePreview();
	}


}

void BinaryOperandImageOperation::ReadRHS(QMimeData* data)
{
	QByteArray byteArray = data->data("application/x-extra-int");
	rhsIndex = byteArray.toInt();

	rhsImage = ImageManager::GetInstance()->GetImage(rhsIndex);
	if (lhsIndex != -1 && rhsIndex != -1)
	{
		executionButton->setEnabled(true);
		UpdatePreview();
	}
}

void BinaryOperandImageOperation::DoExecution()
{
	switch (destType->currentIndex())
	{
	case DestType::LHS:
		dest = lhsImage;
		break;
	case DestType::RHS:
		dest = rhsImage;
		break;
	case DestType::NEW_IMAGE:
		dest = CreateNewImage(lhsImage);
		break;
	default:
		return;
	}

	OnExecute();

	auto tab = ImageTabManager::GetInstance()->GetTab(dest->GetIndex());
	tab->UpdateImage(dest);
}

FrequencyDomainOperation::FrequencyDomainOperation(QWidget* parent) : SingleOperandImageOperation(parent)
{
}

void FrequencyDomainOperation::ReadSrc(QMimeData* data)
{
	QByteArray byteArray = data->data("application/x-extra-int");
	index = byteArray.toInt();
	srcImage = ImageManager::GetInstance()->GetImage(index);
	if (index != -1)
	{
		executionButton->setEnabled(true);
		FrequencyUpdatePreview();
	}
}
