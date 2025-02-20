#include "ImageTab.h"

#include <QGraphicsPixmapItem>
#include <qgraphicsscene.h>
#include <QGridLayout>
#include <QTabWidget>

#include "Image.h"
#include "ImageView.h"
#include "ToolBarManager.h"


ImageTab::ImageTab(int index, const QString& name, QTabWidget* tabs, QImage image, QWidget* parent) : QWidget(parent)
{
    QWidget::setObjectName(name);
    layout = new QGridLayout(this);
    graphicsView = new ImageView(this);
    layout->addWidget(graphicsView);

    scene = new QGraphicsScene;
    item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    graphicsView->setScene(scene);
    graphicsView->setRenderHint(QPainter::LosslessImageRendering);
    graphicsView->setAlignment(Qt::AlignCenter);
    graphicsView->show();

    float scale = (static_cast<float>(width()) / static_cast<float>(image.width())) * 2.0f;
    scale = std::min(scale, (static_cast<float>(height()) / static_cast<float>(image.height())) * 2.0f);
    qDebug() << "scale: " << scale;
    graphicsView->scale(scale, scale);
    tabs->addTab(this, name);
    tabs->setCurrentWidget(this);
}

void ImageTab::UpdateImage(Image* image)
{
    if (item) {
        scene->removeItem(item);
        delete item;
    }

    QImage img = image->ToQImage();
    item = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    scene->addItem(item);
}
