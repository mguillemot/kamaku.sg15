#ifndef __FOLIAGE_FASTMATH
#define __FOLIAGE_FASTMATH

#include <cmath>

namespace Foliage
{

	class FastMath
	{
	public:
		static void init();
		static Fixed cos(const Fixed x);
		static Fixed sin(const Fixed x);
		static Fixed atan(const Fixed x);
	
	private:
		FastMath() {}

		static Fixed *atan_t0;
		static Fixed *atan_t1;
		static Fixed *atan_t2;
		static Fixed *atan_t3;
		static Fixed *cos_t;
	};
	
}

#endif //__FOLIAGE_FASTMATH
