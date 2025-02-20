#pragma once
#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <qstring.h>

#include "Singleton.h"

class Image;

namespace Ui
{
	class MainWindow;
}

class ImageLoader : public Singleton<ImageLoader>
{
public:
	void Initialization(Ui::MainWindow* ui);

	void LoadImage(const QString& fileName);

	Image* CreateImage(const QString& name, int width, int height) const;

	static inline int index = 0;

private:

	void OnImagePathTextChanged(const QString& arg1) const;
	void OnLoadImageButtonClicked();
	void OnBrowseImage();

	Ui::MainWindow* ui;
};

#endif // IMAGE_LOADER_H