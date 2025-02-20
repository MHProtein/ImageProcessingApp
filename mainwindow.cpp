#include "mainwindow.h"

#include "ImageLoader.h"
#include "ImageTabManager.h"
#include "Opeartions.h"
#include "ToolBarManager.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->LoadImageButton->setEnabled(false);

    ImageLoader::GetInstance()->Initialization(ui);
    ImageTabManager::GetInstance()->Initialization(ui->ImageTabs);
    ToolBarManager::GetInstance()->Initialization(ui);
    ui->horizontalLayout_3->setAlignment(Qt::AlignTop);
    ui->horizontalLayout_4->setAlignment(Qt::AlignTop);
    ui->horizontalLayout_5->setAlignment(Qt::AlignTop);
}

MainWindow::~MainWindow()
{
    delete ui;
}