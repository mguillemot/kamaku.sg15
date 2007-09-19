#include "star.hpp"

Star::Star()
{
    recreate();
    _y = (float)(rand() % Foliage::Screen::Height);
}

void Star::update()
{
    _y += _speed;
    if (_y > Foliage::Screen::Height)
    {
        recreate();
    }
}

void Star::display() const
{
    Foliage::Rect r;
    r.x = _x;
    r.y = (Sint32)_y;
    r.w = 2;
    r.h = 2;
    Foliage::Screen::fillRect(r, Foliage::Colors::White);
}

void Star::recreate()
{
    _x = rand() % Foliage::Screen::Width;
    _y = 0.0f;
    _speed = 1.0f + (float)(rand() % 100) / 10.0f;
}
