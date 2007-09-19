#ifndef __FOLIAGE_GRAPHIC_TYPES
#define __FOLIAGE_GRAPHIC_TYPES

#include "../basic_types.hpp"
#include "../settings.hpp"

namespace Foliage
{

	typedef Uint16 Color;

	enum ScreenMode { YOKO, TATE };

	struct Size
	{
		Sint16 w, h;
		
		Size() : w(0), h(0)
		{
		}
		
		Size(const Sint16 sw, const Sint16 sh)
			: w(sw), h(sh)
		{
		}
	};
	
	struct Point
	{
		Sint16 x, y;
		
		Point() : x(0), y(0)
		{
		}
		
		Point(const Sint16 px, const Sint16 py)
			: x(px), y(py)
		{
		}
	};
	
	struct HwPoint
	{
		Sint16 x, y;
		
		HwPoint() : x(0), y(0)
		{
		}
		
		HwPoint(const Sint16 px, const Sint16 py)
			: x(px), y(py)
		{
		}
		
		HwPoint(const Point p)
		{
			#ifdef SCREEN_VERTICAL
			x = SCREEN_WIDTH - 1 - p.y;
			y = p.x;
			#else
			x = p.x;
			y = p.y;
			#endif
		}
		
		Point toPoint() const
		{
			#ifdef SCREEN_VERTICAL
			return Point(y, SCREEN_WIDTH - 1 - x);
			#else
			return Point(x, y);
			#endif		
		}
	};
	
	struct Speed
	{
		Fixed x, y;
		
		Speed() : x(), y()
		{
		}
		
		Speed(const Fixed &sx, const Fixed &sy)
			: x(sx), y(sy)
		{
		}
	};

	struct Rect
	{
		Sint16 x, y;
		Sint16 w, h;
		
		Rect() : x(0), y(0), w(0), h(0)
		{
		}
		
		Rect(const Sint16 rx, const Sint16 ry, const Sint16 rw, const Sint16 rh)
			: x(rx), y(ry), w(rw), h(rh)
		{
		}
		
		Rect(const Point p, const Size s)
			: x(p.x), y(p.y), w(s.w), h(s.h)
		{
		}
		
		static bool intersects(const Rect &a, const Rect &b);
	};

	inline bool Rect::intersects(const Foliage::Rect &a, const Foliage::Rect &b)
	{
		return !(b.x > a.x + a.w || b.x + b.w < a.x || b.y > a.y + a.h || b.y + b.h < a.y);
	}
	
}

#endif //__FOLIAGE_GRAPHIC_TYPES
