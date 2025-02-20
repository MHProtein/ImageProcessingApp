#include "FloatSlider.h"

#include <QLabel>

FloatSlider::FloatSlider(float minValue, float maxValue, float initialValue, QWidget* parent, const std::function<void(float)>& callback) :
	QWidget(parent), minFloat(minValue), maxFloat(maxValue), callback(callback)
{
	auto* layout = new QHBoxLayout(this);
	slider = new QSlider(Qt::Horizontal, this);
	valueDisplay = new QLineEdit(this);

	slider->setRange(0, sliderSteps); 
	int initialSliderValue = FloatToSliderValue(initialValue);
	slider->setValue(initialSliderValue);

	valueDisplay->setText(QString::number(initialValue, 'f', 2)); 
	auto* validator = new QDoubleValidator(minValue, maxValue, 2, this); 
	valueDisplay->setValidator(validator);
	valueDisplay->setFixedWidth(60); 

	layout->addWidget(slider);
    layout->addWidget(valueDisplay);
	connect(slider, &QSlider::valueChanged, this, &FloatSlider::UpdateText);
	connect(valueDisplay, &QLineEdit::editingFinished, this, &FloatSlider::UpdateSlider);
}

void FloatSlider::UpdateText(int intSliderValue)
{
	float floatValue = SliderToFloatValue(intSliderValue);
	valueDisplay->setText(QString::number(floatValue, 'f', 2));
	OnValueChanged(floatValue);
}

void FloatSlider::UpdateSlider()
{
	bool ok;
	float floatValue = valueDisplay->text().toFloat(&ok);
	if (ok && floatValue >= minFloat && floatValue <= maxFloat) 
	{
		int intSliderValue = FloatToSliderValue(floatValue);
		slider->setValue(intSliderValue);
	}
}

void FloatSlider::OnValueChanged(float value)
{
	if(callback)
		callback(value);
}

int FloatSlider::FloatToSliderValue(float value) const
{
	return static_cast<int>((value - minFloat) / (maxFloat - minFloat) * sliderSteps);
}

float FloatSlider::SliderToFloatValue(int sliderValue) const
{
	return minFloat + (static_cast<float>(sliderValue) / sliderSteps) * (maxFloat - minFloat);
}

IntSlider::IntSlider(int minValue, int maxValue, int initialValue, bool oddOnly, const QString& labelText, QWidget* parent,
	const std::function<void(int)>& callback): QWidget(parent), callback(callback), oddOnly(oddOnly), minValue(minValue), maxValue(maxValue)
{
	auto* layout = new QHBoxLayout(this);
	auto* label = new QLabel(labelText, this);
	slider = new QSlider(Qt::Horizontal, this);
	valueDisplay = new QLineEdit(this);

	slider->setRange(minValue, maxValue);
	slider->setValue(initialValue);

	valueDisplay->setText(QString::number(initialValue));
	valueDisplay->setValidator(new QIntValidator(minValue, maxValue, this));
	valueDisplay->setFixedWidth(50); 

	layout->addWidget(label);
	layout->addWidget(slider);
	layout->addWidget(valueDisplay);

	connect(slider, &QSlider::valueChanged, this, &IntSlider::UpdateOddValue);
	connect(valueDisplay, &QLineEdit::editingFinished, this, &IntSlider::updateSliderFromText);
}

void IntSlider::UpdateOddValue(int value) {
	if(oddOnly)
	{
		if (value % 2 == 0) {
			value += 1;
			slider->setValue(value);
			OnValueChanged(value);
		}
	}
	else
	{
				slider->setValue(value);
		OnValueChanged(value);
	}

	valueDisplay->blockSignals(true);
	valueDisplay->setText(QString::number(value));
	valueDisplay->blockSignals(false);
}

void IntSlider::updateSliderFromText() {
	bool ok;
	int value = valueDisplay->text().toInt(&ok);
	if(oddOnly)
	{
		if (ok && value % 2 != 0 && value >= minValue && value <= maxValue) {
			slider->setValue(value);
			OnValueChanged(value);
		}
	}
	else
	{
		if(ok && value >= minValue && value <= maxValue)
		{
			slider->setValue(value);
			OnValueChanged(value);
		}
	}

}
void IntSlider::OnValueChanged(int value) const
{
	if (callback)
		callback(value);
}
