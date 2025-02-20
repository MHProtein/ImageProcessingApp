#include "ImageLoader.h"

#include <QFileDialog>
#include <QMessageBox>
#include <ui_mainwindow.h>
#include <opencv2/opencv.hpp>

#include "ImageManager.h"
#include "ImageTab.h"
#include "ImageTabManager.h"
#include "ToolBarManager.h"

void ImageLoader::Initialization(Ui::MainWindow* ui)
{
	this->ui = ui;

	connect(ui->ImagePath, &QLineEdit::textChanged, this, &ImageLoader::OnImagePathTextChanged);
	connect(ui->LoadImageButton, &QPushButton::clicked, this, &ImageLoader::OnLoadImageButtonClicked);
	connect(ui->BrowseImagePath, &QPushButton::clicked, this, &ImageLoader::OnBrowseImage);
}

void ImageLoader::LoadImage(const QString& fileName)
{
	auto path = fileName.toStdString();
	auto image = cv::imread(path, cv::IMREAD_GRAYSCALE);

	if (image.empty())
	{
		QMessageBox::warning(nullptr, "Error", "Failed to load image");
		return;
	}
	ui->ImagePath->clear();

	auto name = fileName.split('/').back();
	auto newImage = new Image(image, index);
	ImageTabManager::GetInstance()->AddTab(index, new ImageTab(index, name, ui->ImageTabs, newImage->ToQImage(), nullptr));
	ImageManager::GetInstance()->AddImage(index, newImage);
	ToolBarManager::GetInstance()->AddItem(index, new QListWidgetItem(name));
	index++;
}

Image* ImageLoader::CreateImage(const QString& name, int width, int height) const
{
	cv::Mat image = cv::Mat::ones(height, width, CV_8UC1);
	auto newImage = new Image(image, index);
	ImageTabManager::GetInstance()->AddTab(index, new ImageTab(index, name, ui->ImageTabs, newImage->ToQImage(), nullptr));
	ImageManager::GetInstance()->AddImage(index, newImage);
	ToolBarManager::GetInstance()->AddItem(index, new QListWidgetItem(name));
	index++;
	return newImage;
}

void ImageLoader::OnImagePathTextChanged(const QString& arg1) const
{
	if (arg1.isEmpty())
	{
		ui->LoadImageButton->setEnabled(false);
	}
	else
	{
		ui->LoadImageButton->setEnabled(true);
	}
}

void ImageLoader::OnLoadImageButtonClicked()
{
	auto imagePath = ui->ImagePath->text();
	LoadImage(imagePath);
}

void ImageLoader::OnBrowseImage()
{
	QString path = QFileDialog::getOpenFileName(nullptr, "Open a file", "D:\GitHub\ImageProcessingApp\Images",
		"Images (*.png *.jpg);;");
	ui->ImagePath->setText(path);
	LoadImage(path);
}
