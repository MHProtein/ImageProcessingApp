#include "ImageListWidget.h"

#include <qmenu.h>
#include <QContextMenuEvent>
#include <QMimeData>
#include <QDrag>
#include <QFileDialog>

#include "ImageManager.h"
#include "ImageTab.h"
#include "ImageTabManager.h"
#include "ToolBarManager.h"

ImageListWidget::ImageListWidget(QWidget* parent) : QListWidget(parent)
{
    
}

void ImageListWidget::contextMenuEvent(QContextMenuEvent* event)
{
    QListWidgetItem* item = itemAt(event->pos());
    if (!item) return;

    QMenu contextMenu(this);
    QAction* deleteAction = contextMenu.addAction("Delete");

    QAction* saveAction = contextMenu.addAction("Save");

    QAction* resetAction = contextMenu.addAction("Reset");

    connect(deleteAction, &QAction::triggered, this, [this, item]() {
        HandleDelete(item);
    });

	connect(saveAction, &QAction::triggered, this, [this, item]() {
		HandleSave(item);
    });


    connect(resetAction, &QAction::triggered, this, [this, item]() {
        HandleReset(item);
    });

    contextMenu.exec(event->globalPos());

}

void ImageListWidget::startDrag(Qt::DropActions supportedActions)
{
    QListWidgetItem* item = currentItem();
    if (item) 
    {
        QMimeData* mimeData = new QMimeData;
        mimeData->setText(item->text());

    	QByteArray byteArray;
        byteArray.setNum(ToolBarManager::GetInstance()->GetIndex(item));
        mimeData->setData("application/x-extra-int", byteArray);

        QDrag* drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->exec(Qt::CopyAction);
    }

}

void ImageListWidget::HandleDelete(QListWidgetItem* item)
{
	auto index = ToolBarManager::GetInstance()->GetIndex(item);
	ToolBarManager::GetInstance()->RemoveItem(index);
    ImageTabManager::GetInstance()->RemoveTab(index);
    ImageManager::GetInstance()->RemoveImage(index);
}

void ImageListWidget::HandleSave(QListWidgetItem* item)
{
    auto index = ToolBarManager::GetInstance()->GetIndex(item);
    auto image = ImageManager::GetInstance()->GetImage(index);

    auto path = QFileDialog::getSaveFileName(nullptr, "Pick a location to store the image!", 
        "D:\GitHub\ImageProcessingApp\Images", "Images (*.png *.jpg);;");

    if (path.isEmpty())
        return;

    image->SaveImage(path.toStdString());
}

void ImageListWidget::HandleReset(QListWidgetItem* item)
{
	auto index = ToolBarManager::GetInstance()->GetIndex(item);
	auto image = ImageManager::GetInstance()->GetImage(index);
	image->ResetImageToOriginal();
	ImageTabManager::GetInstance()->GetTab(index)->UpdateImage(image);
}
