#ifndef __FOLIAGE_INSTANCIZATOR
#define __FOLIAGE_INSTANCIZATOR

#include "surface.hpp"

#define INSTANCIZATOR_MAX 64

namespace Foliage
{
	
	class Instancizator
	{
	public:
		static void instancize(Surface *surf);
		
	private:
		Instancizator() { }
	
		static Surface* _instances[INSTANCIZATOR_MAX];
		static Sint32 _next_free_instance;
	};
	
}

#endif //__FOLIAGE_INSTANCIZATOR
