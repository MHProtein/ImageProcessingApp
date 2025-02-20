#pragma once
#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H
#include "QGraphicsView"

class ImageView : public QGraphicsView 
{
	Q_OBJECT
public:
	explicit ImageView(QWidget* parent = nullptr);

protected:
	bool isZoom = false;
	double scaleFactor = 1.2;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;
	using QGraphicsView::QGraphicsView;
};

#endif // IMAGE_VIEW_H