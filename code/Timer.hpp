#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <Common/timer.hpp>
#include <time.h>

class Timer : public QGAMES::Timer
{
	public:
	Timer ()
		: QGAMES::Timer ()
							{ }

	private:
	long actualMilliSeconds () const
							{ return ((long) clock ()); }
};

#endif