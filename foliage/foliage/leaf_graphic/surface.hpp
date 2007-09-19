#ifndef _FOLIAGE__SURFACE
#define _FOLIAGE__SURFACE

#include <string>
#include "../basic_types.hpp"
#include "graphic_types.hpp"

namespace Foliage
{

	class Surface
	{
	public:
		// Constructor & destructor
		~Surface();
		
		// Instance members
		Color  getPixel(const Point p) const;
		void   setPixel(const Point p, const Color color);
		void   drawAt(const Point p) const;
		Size   getSize() const;
		const  std::string &getName() const;
		void   setInstancized(const Sint32 instancized) { _instancized = instancized; }
		Sint32 getInstancized() const { return _instancized; }
		
		// DEBUG
		Color *getPixels() const { return _pixels; }
		Surface *createNewShiftedSurface(const Sint32 shift) const;

		// Static members
		static Surface *readBMP(const std::string &filename);

	private:
		Surface(const Size s, Color *pixels, const std::string &name);

		Size _size;
		Color *_pixels;
		std::string _name;
		Sint32 _instancized;		
	};
	
}

#endif //_FOLIAGE__SURFACE
