#pragma once
#include <QMimeData>
#include <qtmetamacros.h>

#include "ImageListWidget.h"

class QLabel;

class ImageListItemContainerWidget : public QWidget
{
	Q_OBJECT

public:
	int index;
	explicit ImageListItemContainerWidget(QWidget *parent = nullptr, const std::function<void(QMimeData*)>& dropCallback = nullptr);

protected:
	std::function<void(QMimeData*)> callback;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

private:
	QLabel* label;

};

