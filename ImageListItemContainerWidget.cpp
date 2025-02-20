#include "ImageListItemContainerWidget.h"

#include <qevent.h>
#include <QLabel>
#include <QMimeData>

ImageListItemContainerWidget::ImageListItemContainerWidget(QWidget* parent, const std::function<void(QMimeData*)>& dropCallback) : QWidget(parent), callback(dropCallback)
{
	setAcceptDrops(true);
	label = new QLabel("Drop item here", this);
	label->setFrameShape(QFrame::Panel);
	label->setAlignment(Qt::AlignCenter);
}

void ImageListItemContainerWidget::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasText()) 
	{
		event->acceptProposedAction();
	}
}

void ImageListItemContainerWidget::dropEvent(QDropEvent* event)
{
	QString itemText = event->mimeData()->text();
	label->setText(itemText);

	if(callback)
		callback(const_cast<QMimeData*>(event->mimeData()));

	event->acceptProposedAction();
}
