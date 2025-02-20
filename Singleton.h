#pragma once
#include <QObject>

template <typename T>
class Singleton : public QObject
{
	public:
		static T* GetInstance();
};

template <typename T>
T* Singleton<T>::GetInstance()
{
	static T instance;
	return &instance;
}

