/** 
 *
 *
 */
#ifndef EDGE_WIN32TIMER_H
#define EDGE_WIN32TIMER_H

#include "EdgeTimer.h"
#include <windows.h>

namespace Edge
{
	/**
	 *
	 *
	 */
	class Win32Timer : public Timer
	{
	public:
		/**
		 *
		 *
		 */
		Win32Timer();

		/**
		 *
		 *
		 */
		~Win32Timer();

		/** 
		 *
		 *
		 */
		bool initiate();

		/**
		 *
		 *
		 */
		void release();

		/** 
		 *
		 */
		unsigned long getCurTime();

		/**
		 *
		 */
		unsigned long getRunTime();

	private:
		DWORD mStartTime;
	};
}

#endif