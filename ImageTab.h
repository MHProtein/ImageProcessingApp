#pragma once
#include <QWidget>

class QGraphicsScene;
class QGraphicsPixmapItem;
class Image;
class ImageView;
class QTabWidget;
class QGraphicsView;
class QGridLayout;

class ImageTab : public QWidget
{
	Q_OBJECT

public:
	int index = 0;
	ImageTab(int index, const QString& name, QTabWidget* tabs, QImage image, QWidget* parent);
	void UpdateImage(Image* image);
protected:
	QGridLayout *layout;
	ImageView *graphicsView;
	QGraphicsScene* scene;
	QGraphicsPixmapItem* item;
};

