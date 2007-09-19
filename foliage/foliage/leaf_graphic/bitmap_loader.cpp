#include "bitmap_loader.hpp"

Foliage::stringMap Foliage::BitmapLoader::surfaces;

Foliage::Surface *Foliage::BitmapLoader::loadBitmap(const std::string &bitmap)
{
	stringMap::iterator i = surfaces.find(bitmap.c_str());
	if (i == surfaces.end())
	{
		// asked bitmap is not loaded yet
		Foliage::Surface *s = Foliage::Surface::readBMP(bitmap);
		surfaces[bitmap.c_str()] = s;
		return s;
	}
	else
	{
		return (*i).second;
	}
}
