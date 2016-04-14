#ifndef SINGLE_H
#define SINGLE_H
#include <memory>
#include <new>
#include <assert.h>
#include <functional>
#include "Config.h"
#include "Threads.h"

typedef std::function<void()> atexit_t;
template <typename T>
struct CreateUsingNew
{
	static T* Create()
	{
		return new T;
	}

	static void Destroy(T* p)
	{
		delete p;
	}
};

template<template<class> class Alloc>
struct CreateUsingAlloc
{
	template <class T>
	struct Allocator
	{
		static Alloc<T> allocator;

		static T* Create()
		{
			return new (allocator.allocate(1)) T;
		}

		static void Destroy(T* p)
		{
			//allocator.destroy(p);
			p->~T();
			allocator.deallocate(p, 1);
		}
	};
};
template <typename T>
struct DefaultLiftTime
{
	static void DeadReference()
	{
		throw std::logic_error("Dead Reference!");
	}

	static void ScheduleLifePolicy(atexit_t pfun)
	{
		std::atexit(pfun);
	}
};



//template <typename T,
//		  template<typename> class CreateUsing,
//		  template<typename> class LifeTime,
//		  template<typename> class ThreadPolicy
//         >
template <typename T,
	template <typename> class CreateUsing = CreateUsingNew,
	template <typename> class LifeTime = DefaultLiftTime,
	template <typename, typename> class ThreadPolicy = ObjectThread
>
class Singleton :private NoCopyable
{
public:
	typedef T ObjectType;
	static T& Instance();

protected:
	static void CreateInstance();
	static void __cdecl DestroySingleton();

	Singleton();

	typedef typename ThreadPolicy<std::T*, GUARD::Lock>::VolatileType pInstance;
	static pInstance _pinstance;
	static bool _destroyed;
};

template <typename T,
	template <typename> class U,
	template <typename> class X,
	template <typename, typename> class Y
>
typename Singleton<T, U, X, Y>::pInstance Singleton<T, U, X, Y>::_pinstance = nullptr;

template <typename T,
	template <typename> class U,
	template <typename> class X,
	template <typename, typename> class Y
>
bool Singleton<T, U, X, Y>::_destroyed = false;

template <typename T,
	template <typename> class CreateUsing,
	template <typename> class LifeTime,
	template <typename, typename> class ThreadPolicy
>
inline T& Singleton<T, CreateUsing, LifeTime, ThreadPolicy>::Instance()
{
	if (!_pinstance)
	{
		CreateInstance();
	}
	return *_pinstance;
}

template <typename T,
	template <typename> class CreateUsing,
	template <typename> class LifeTime,
	template <typename, typename> class ThreadPolicy
>
void Singleton<T, CreateUsing, LifeTime, ThreadPolicy>::CreateInstance()
{
	GUARD::Lock _guard;
	_guard.lock();

	if (!_pinstance)
	{
		if (_destroyed)
		{
			_destroyed = false;
			LifeTime<T>::DeadReference();
		}
		_pinstance = CreateUsing<T>::Create();
		LifeTime<T>::ScheduleLifePolicy(&DestroySingleton);
	}
}

template <typename T,
	template <typename> class CreateUsing,
	template <typename> class LifeTime,
	template <typename, typename> class ThreadPolicy
>
void Singleton<T, CreateUsing, LifeTime, ThreadPolicy>::DestroySingleton()
{
	assert(!_pinstance);
	CreateUsing<T>::Destroy(_pinstance);
	_destroyed = true;
	_pinstance = nullptr;
}


#endif