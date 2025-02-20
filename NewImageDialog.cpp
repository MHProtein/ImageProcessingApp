#include "NewImageDialog.h"
#include <QVBoxLayout>
#include <QLabel>

NewImageDialog::NewImageDialog(QWidget* parent)
    : QDialog(parent), imageText("")
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel("Enter text for the new image:", this);
    layout->addWidget(label);

    textInput = new QLineEdit(this);
    layout->addWidget(textInput);

    confirmButton = new QPushButton("Confirm", this);
    layout->addWidget(confirmButton);

    connect(confirmButton, &QPushButton::clicked, this, &NewImageDialog::OnConfirmButtonClicked);

    setWindowTitle("Create New Image");
    setLayout(layout);
}

void NewImageDialog::OnConfirmButtonClicked()
{
    imageText = textInput->text(); 
    accept();
}

QString NewImageDialog::GetImageText() const
{
    return imageText;
}