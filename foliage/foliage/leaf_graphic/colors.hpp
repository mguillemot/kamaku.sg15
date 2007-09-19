#ifndef __FOLIAGE_COLORS
#define __FOLIAGE_COLORS

#include "graphic_types.hpp"

namespace Foliage
{

	class Colors
	{
	public:
		static const Color Black = 0x0000;
		static const Color Red = 0xF800;
		static const Color Green = 0x07E0;
		static const Color Blue = 0x001F;
		static const Color Pink = 0xF81F;
		static const Color LightBlue = 0x07FF;
		static const Color Yellow = 0xFFE0;
		static const Color White = 0xFFFF;

		static const Color Transparent = Pink;
		
	private:
		Colors() {}
	};
	
}

#endif //__FOLIAGE_COLORS
