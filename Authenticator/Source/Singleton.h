#pragma once

template<class T>
class Singleton
{
public:

	static T& GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new T();
		}

		return *instance;
	}

	static void Release()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

protected:

	Singleton() {};
	~Singleton() { Release(); };

	static T* instance;
};

template <class T>
T* Singleton<T>::instance = nullptr;

