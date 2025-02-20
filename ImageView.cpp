#include "ImageView.h"
#include "QWheelEvent"

ImageView::ImageView(QWidget* parent) : QGraphicsView(parent)
{
}

void ImageView::keyPressEvent(QKeyEvent* event)
{
	QGraphicsView::keyPressEvent(event);
	if (event->key() == Qt::Key_Control)
		isZoom = true;

}

void ImageView::keyReleaseEvent(QKeyEvent* event)
{
	QGraphicsView::keyReleaseEvent(event);
	if (event->key() == Qt::Key_Control)
		isZoom = false;
}

void ImageView::wheelEvent(QWheelEvent* event)
{
	if(!isZoom)
	{
		QGraphicsView::wheelEvent(event);
		return;
	}
	setTransformationAnchor(AnchorUnderMouse);

	if(event->angleDelta().y() > 0)
	{
		scale(scaleFactor, scaleFactor);
	}
	else
	{
		scale(1.0 / scaleFactor, 1.0 / scaleFactor);
	}
}
