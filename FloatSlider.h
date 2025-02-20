
#include <QSlider>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QLabel>

class FloatSlider : public QWidget
{
    Q_OBJECT

public:
    FloatSlider(float minValue, float maxValue, float initialValue, QWidget* parent = nullptr, const std::function<void(float)>& callback = nullptr);

private slots:
    void UpdateText(int intSliderValue);

    void UpdateSlider();

    void OnValueChanged(float value);

private:
    QSlider* slider;
    QLineEdit* valueDisplay;
    float minFloat;
    float maxFloat;
    const int sliderSteps = 1000;
    std::function<void(float)> callback;

    int FloatToSliderValue(float value) const;

    float SliderToFloatValue(int sliderValue) const;

};

class IntSlider : public QWidget
{
    Q_OBJECT

public:
    IntSlider(int minValue, int maxValue, int initialValue, bool oddOnly, const QString& labelText, 
        QWidget* parent = nullptr, const std::function<void(int)>& callback = nullptr);


private slots:
    void UpdateOddValue(int value);
    void updateSliderFromText();

    void OnValueChanged(int value) const;
private:
    QSlider* slider;
    QLineEdit* valueDisplay;
    std::function<void(float)> callback;
    int minValue;
    int maxValue;
    bool oddOnly;
};