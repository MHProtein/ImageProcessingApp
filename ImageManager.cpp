#include "ImageManager.h"

void ImageManager::AddImage(int index, Image* image)
{
	images.insert(std::make_pair(index, image));
}

void ImageManager::RemoveImage(Image* image)
{
	for (auto it = images.begin(); it != images.end(); ++it)
	{
		if (it->second == image)
		{
			images.erase(it);
			break;
		}
	}
}

void ImageManager::RemoveImage(int index)
{
	images.erase(index);
}

Image* ImageManager::GetImage(int index)
{
	auto it = images.find(index);
	return it != images.end() ? it->second : nullptr;
}
