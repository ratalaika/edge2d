/**
 *
 *
 */
#include "EdgeWin32Timer.h"

namespace Edge
{
	Win32Timer::Win32Timer()
	{
	}

	Win32Timer::~Win32Timer()
	{
		release();
	}

	bool Win32Timer::initiate()
	{
		mStartTime = timeGetTime();

		return true;
	}

	void Win32Timer::release()
	{
	}

	unsigned long Win32Timer::getCurTime()
	{
		return timeGetTime();
	}

	unsigned long Win32Timer::getRunTime()
	{
		return (timeGetTime() - mStartTime) ;
	}

}