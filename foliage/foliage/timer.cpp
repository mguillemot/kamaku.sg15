#include "timer.hpp"

#ifdef __PPC__
	#include <xparameters.h>
#endif

Foliage::Timer::Timer() : _started(0), _stopped(0)
{
}

void Foliage::Timer::start()
{
	if (_stopped == 0)
	{
		XTime_GetTime(&_started);
	}
	else
	{
		XTime now;
		XTime_GetTime(&now);
		_started = now - (_stopped - _started);
		_stopped = 0;
	}
}

void Foliage::Timer::stop()
{
	XTime_GetTime(&_stopped);
}

float Foliage::Timer::duration() const
{
	if (_started == 0)
	{
		return 0.0f;
	}
	if (_stopped == 0)
	{
		XTime now;
		XTime_GetTime(&now);
		return (float)(now - _started) / XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ;
	}
	else
	{
		return (float)(_stopped - _started) / XPAR_CPU_PPC405_CORE_CLOCK_FREQ_HZ;
	}
}

void Foliage::Timer::reset()
{
	_started = 0;
	_stopped = 0;
}
