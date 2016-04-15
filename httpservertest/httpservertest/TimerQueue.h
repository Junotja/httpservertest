#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "Guard.h"

template <typename T = LARGE_INTEGER>
class Timer
{
public:
	Timer(T& x);
	~Timer();

	__int64 getnow();

private:
	__int64 _frequency;
};

template <typename T>
Timer<T>::Timer(T& x)
{
	//_frequency = TimerHelper<QueryPerformanceFrequency(&x)>::frequency(x);
}

template <typename T>
Timer<T>::~Timer()
{

}

template <bool>
struct TimerHelper
{
	template <typename T>
	static __int64 frequency(T& x)
	{
		return x.QuadPart;
	}
};

template<>
struct TimerHelper<false>
{
	template <typename T>
	static __int64 frequency(T& x)
	{
		return 0;
	}
};

#endif