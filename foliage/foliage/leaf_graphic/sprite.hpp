#ifndef __FOLIAGE_SPRITE
#define __FOLIAGE_SPRITE

#include <string>
#include <list>
#include "../basic_types.hpp"
#include "graphic_types.hpp"
#include "surface.hpp"

namespace Foliage
{

	class Sprite
	{
	public:	
		// Constructor & destructor
		Sprite(const std::string &filename);
		Sprite(Surface *surface);
		~Sprite();
		
		// Instance methods
		void  addFrame(const std::string &filename);
		void  draw() const;
		void  move();
		void  setPosition(const Point p);
		Point getPosition() const;
		Point getCenter() const;
		void  setSpeed(const Speed s);
		Speed getSpeed() const;
		bool  outOfScreen() const;
		Rect  getScreenHitbox() const;
		void  setHitbox(const Rect &hitbox);
		Size  getSize() const;
		bool  isAnimated() const;
		
		// DEBUG
		Surface *getCurrentSurface() { return *_currentFrame; }
		void inspect();
		
	private:
		Fixed _x, _y;
		Speed _s;
		std::list<Surface *> _frames;
		std::list<Surface *>::iterator _currentFrame;
		Rect _hitbox;
	};

}

#endif //__FOLIAGE_SPRITE
