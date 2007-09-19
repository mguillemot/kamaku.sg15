#ifndef _FOLIAGE__BITMAP_LOADER
#define _FOLIAGE__BITMAP_LOADER

#include <string>
#include <map>
#include "surface.hpp"

namespace Foliage
{

	typedef std::map<const char*, Surface*> stringMap;

	class BitmapLoader
	{
	public:
		static Surface *loadBitmap(const std::string &bitmap);
	
	private:
		static stringMap surfaces;
	};	
	
}

#endif //_FOLIAGE__BITMAP_LOADER
