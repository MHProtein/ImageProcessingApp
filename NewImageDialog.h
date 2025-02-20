#ifndef NEWIMAGEDIALOG_H
#define NEWIMAGEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class NewImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewImageDialog(QWidget* parent = nullptr);
    QString GetImageText() const;  // Function to get the entered text

private slots:
    void OnConfirmButtonClicked();

private:
    QLineEdit* textInput;
    QPushButton* confirmButton;
    QString imageText;
};

#endif // NEWIMAGEDIALOG_H

