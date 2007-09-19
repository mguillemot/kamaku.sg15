#ifndef _FOLIAGE__FOLIAGE
#define _FOLIAGE__FOLIAGE

#include "basic_types.hpp"
#include "timer.hpp"
#include "memory.hpp"
#include "fastmath.hpp"
#include "leaf_graphic/leaf_graphic.hpp"
#include "leaf_io/leaf_io.hpp"
#include "leaf_sound/leaf_sound.hpp"

namespace Foliage
{

	void init();
	void shutDown();
	void flushDCache();
}

#endif //_FOLIAGE__FOLIAGE
