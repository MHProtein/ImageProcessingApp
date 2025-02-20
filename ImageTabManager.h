#pragma once
#include <unordered_map>

#include "Singleton.h"

class QTabWidget;
class ImageTab;

class ImageTabManager : public Singleton<ImageTabManager>
{
public:
	void Initialization(QTabWidget* tabsUI);
	void AddTab(int index, ImageTab* tab);
	void RemoveTab(int index);
	void RemoveTab(ImageTab* tab);
	ImageTab* GetTab(int index);
	int GetTabIndex(ImageTab* tab);
	int GetCurrentIndex();
	bool isCurrentTab(int imageIndex);
	void SetTabIndex(int index);

	void SwitchToTab(int index);
private:

	void CloseTab(int index) const;

	QTabWidget* tabsUI;
	std::unordered_map<int, ImageTab*> tabs;
};

