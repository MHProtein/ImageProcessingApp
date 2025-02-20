#pragma once
#include "Image.h"
#include "Singleton.h"

class ImageManager : public Singleton<ImageManager>
{
public:
	void AddImage(int index, Image* image);
	void RemoveImage(Image* image);
	void RemoveImage(int index);

	Image* GetImage(int index);

private:

	std::unordered_map<int, Image*> images;
};
