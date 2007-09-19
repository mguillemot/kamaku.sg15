#include <iostream>
#include "bullet.hpp"
#include "fastmath.hpp"

Foliage::Surface *Bullet::_bulletSurface = NULL;

Bullet::Bullet(Foliage::Point position, Foliage::Fixed direction, Foliage::Fixed speed)
	: _direction(direction)
{
	if (_bulletSurface == NULL)
	{
		_bulletSurface = Foliage::BitmapLoader::loadBitmap("bullet.bmp");
		Foliage::Instancizator::instancize(_bulletSurface);
	}
    _sprite = new Foliage::Sprite(_bulletSurface);
    Foliage::Size size = _sprite->getSize();
    Foliage::Rect bouletteHitbox = Foliage::Rect(2, 2, 4, 4);
    _sprite->setHitbox(bouletteHitbox);
    _sprite->setPosition(Foliage::Point(position.x - (size.w >> 1), position.y - (size.h >> 1)));
    setSpeed(speed);
}

Bullet::~Bullet()
{
    delete _sprite;
}

void Bullet::updateSpriteSpeed()
{
    Foliage::Fixed sx = _speed * Foliage::FastMath::cos(_direction);
    Foliage::Fixed sy = _speed * Foliage::FastMath::sin(_direction);
    _sprite->setSpeed(Foliage::Speed(sx, sy));
}

void Bullet::update()
{
    _sprite->move();
}

Foliage::Sprite *Bullet::getSprite()
{
    return _sprite;
}

Foliage::Fixed Bullet::getSpeed()
{
    return _speed;
}

Foliage::Fixed Bullet::getDirection()
{
    return _direction;
}

void Bullet::setSpeed(Foliage::Fixed speed)
{
    _speed = speed;
    updateSpriteSpeed();
}

void Bullet::setDirection(Foliage::Fixed direction)
{
    _direction = direction;
    updateSpriteSpeed();
}
