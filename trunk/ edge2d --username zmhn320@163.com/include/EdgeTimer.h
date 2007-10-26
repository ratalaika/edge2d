/**
 *
 *
 */
#ifndef EDGE_TIMER_H
#define EDGE_TIMER_H

#include "EdgeCompile.h"

namespace Edge
{
	/**
	 * A Timer.
	 *
	 */
	class EDGE_EXPORT Timer
	{
	public:
		/**
		 * Constructor
		 *
		 */
		Timer() {}

		/**
		 * Destructor
		 *
		 */
		virtual ~Timer() {}

		/**
		 * initiate
		 * 
		 */
		virtual bool initiate() = 0;

		/**
		 * release, called by the destructor.
		 *
		 */
		virtual void release() = 0;

		/**
		 * reset the timer
		 *
		 */
		virtual void reset() {}

		/**
		 * getCurTime, get the current time in millseconds.
		 *
		 */
		virtual unsigned long getCurTime() = 0;

		/**
		 * getRunTime, get the time the applicaton has gone.
		 *
		 */
		virtual unsigned long getRunTime() = 0;
	};
}

#endif