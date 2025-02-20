#include "ImageTabManager.h"

#include <QTabWidget>

#include "ImageTab.h"

void ImageTabManager::Initialization(QTabWidget* tabsUI)
{
	this->tabsUI = tabsUI;
	connect(tabsUI, &QTabWidget::tabCloseRequested, this, &ImageTabManager::CloseTab);
}

void ImageTabManager::AddTab(int index, ImageTab* tab)
{
	tabs.insert(std::make_pair(index, tab));
}

void ImageTabManager::RemoveTab(int index)
{
	tabsUI->removeTab(tabsUI->indexOf(tabs[index]));
	tabs.erase(index);
}

void ImageTabManager::RemoveTab(ImageTab* tab)
{
	tabsUI->removeTab(tabsUI->indexOf(tab));
	for (auto it = tabs.begin(); it != tabs.end(); ++it)
	{
		if (it->second == tab)
		{
			tabs.erase(it);
			break;
		}
	}
}

ImageTab* ImageTabManager::GetTab(int index)
{
	auto it = tabs.find(index);
	return it != tabs.end() ? it->second : nullptr;
}

int ImageTabManager::GetTabIndex(ImageTab* tab)
{
	return tabsUI->indexOf(tab);
}

int ImageTabManager::GetCurrentIndex()
{
	return tabsUI->currentIndex();
}

bool ImageTabManager::isCurrentTab(int imageIndex)
{
	auto tab = GetTab(imageIndex);
	return tab && tabsUI->currentWidget() == tab;
}

void ImageTabManager::SetTabIndex(int index)
{
	tabsUI->setCurrentWidget(tabs[index]);
}

void ImageTabManager::CloseTab(int index) const
{
	tabsUI->removeTab(index);
}

void ImageTabManager::SwitchToTab(int index)
{
	auto tab = GetTab(index);
	if (!tab)
		return;

	if (tabsUI->indexOf(tab) == -1)
		tabsUI->addTab(tab, tab->objectName());
	tabsUI->setCurrentWidget(tab);
}
