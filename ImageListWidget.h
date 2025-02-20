#pragma once
#include <QListWidget>

class ImageListWidget : public QListWidget
{
    Q_OBJECT
public:
	ImageListWidget(QWidget *parent);
	void contextMenuEvent(QContextMenuEvent* event) override;
	void startDrag(Qt::DropActions supportedActions) override;

private:
	void HandleDelete(QListWidgetItem* item);
	void HandleSave(QListWidgetItem* item);
	void HandleReset(QListWidgetItem* item);

};
