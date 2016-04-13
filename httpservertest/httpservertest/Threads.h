#ifndef THREADS_H
#define THREADS_H
#include "Guard.h"

template <typename T,typename ThreadPolicy = GUARD::Lock>
class SingleThread
{
public:
	typedef volatile T VolatileType;
};

template <typename T,typename ThreadPolicy = GUARD::Lock>
class ObjectThread
{
private:
	mutable ThreadPolicy _mutex;

public:
	typedef volatile T VolatileType;

	class Lock :private NoCopyable
	{
	private:
		const ObjectThread& _t;
	public:
		explicit Lock(const ObjectThread& host)
			:_t(host)
		{
			_t._mutex.ock();
		}

		~Lock()
		{
			_t._mutex.unlock();
		}

	};

};
#endif