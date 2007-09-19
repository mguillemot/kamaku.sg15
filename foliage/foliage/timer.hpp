#ifndef _FOLIAGE__TIMER
#define _FOLIAGE__TIMER

#include <xtime_l.h> //TEMP

namespace Foliage
{

	class Timer
	{
	public:
		Timer();
		void start();
		void stop();
		void reset();
		float duration() const;
		
	private:
		XTime _started, _stopped;
	};	
	
}

#endif //_FOLIAGE__TIMER
