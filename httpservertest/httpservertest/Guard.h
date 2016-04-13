#ifndef GUARD_H
#define GUARD_H

#include <windows.h>
#include <memory>
#include <vector>

#include "config.h"

namespace GUARD
{
	//mutex
	class Lock
	{
	private:
		CRITICAL_SECTION _cs;

	public:
		Lock()
		{
			InitializeCriticalSection(&_cs);
		}
		~Lock()
		{
			DeleteCriticalSection(&_cs);
		}

		void lock()
		{
			EnterCriticalSection(&_cs);
		}
		void unlock()
		{
			LeaveCriticalSection(&_cs);
		}
	};

	//rw
	class RWLock
	{
	private:
		int _st;
		int _rlockcount;
		int _rwaitcount;
		int _wwaitcount;
		HANDLE _notifyevent;
		CRITICAL_SECTION _stLock;

	public:
		RWLock(void);
		~RWLock(void);
		void rlock();
		void wlock();
		void unlock();
	};

	//lock pool
	template <typename lock_t>
	class LockPool:private NoCopyable
	{
	public:
		typedef vector<std::shared_ptr<lock_t>> lock_type;
	private:
		size_t   _size;
		//lock_t** _locklist;
		lock_type _locklist;
		size_t   _index;
	public:
		LockPool();
		~LockPool();

		bool init(void);
		bool destroy();

		std::shared_ptr<lock_t> alloc();
		void recycle(std::shared_ptr<lock_t> lockptr);
	};

	template <typename lock_t>
	LockPool<lock_t>::LockPool()
		:_locklist(nullptr),
		_size(0),
		_index(0)
	{

	}

	template <typename lock_t>
	LockPool<lock_t>::~LockPool()
	{

	}

	template <typename lock_t>
	bool LockPool<lock_t>::init()
	{
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		_size = sysinfo.dwNumberOfProcessors;

		//_locklist = std::make_shared<vector<lock_t>>(_size);
		if (_size <= 0)
		{
			return false;
		}

		for (lock_type::iterator pos = _locklist.begin(); pos != _size; ++pos)
		{
			*pos = std::make_shared<lock_t>();
		}

		return true;
	}

	template <typename lock_t>
	bool LockPool<lock_t>::destroy()
	{
		_locklist = nullptr;
		_size = 0;
		_index = 0;
	}

	template <typename lock_t>
	std::shared_ptr<lock_t> LockPool<lock_t>::alloc()
	{
		return _locklist[(_index++) % _size];
	}

	template <typename lock_t>
	void LockPool<lock_t>::recycle(std::shared_ptr<lock_t> lockptr)
	{

	}
}


#endif