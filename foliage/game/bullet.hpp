#ifndef __BULLET
#define __BULLET

#include "foliage.hpp"

class Bullet
{
public:
	// Constructor & destructor
    Bullet(Foliage::Point position, Foliage::Fixed direction, Foliage::Fixed speed);
    ~Bullet();

	// Instance methods
    Foliage::Sprite *getSprite();
    void             setSpeed(Foliage::Fixed speed);
    Foliage::Fixed   getSpeed();
    void             setDirection(Foliage::Fixed direction);
    Foliage::Fixed   getDirection();
    void             update();

private:
    void updateSpriteSpeed();

    Foliage::Fixed _direction;
    Foliage::Fixed _speed;
    Foliage::Sprite *_sprite;
    
    static Foliage::Surface *_bulletSurface;
};

#endif
