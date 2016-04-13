#ifndef SINGLE_H
#define SINGLE_H
#include <memory>
#include <new>
#include <assert.h>
#include "Config.h"
#include "Threads.h"


template <typename T>
struct CreateUsingNew
{
	static std::shared_ptr<T> Create()
	{
		return std::make_shared<T>();
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

	typedef typename ThreadPolicy<std::shared_ptr<T>, GUARD::Lock>::VolatileType pInstance;
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



#endif