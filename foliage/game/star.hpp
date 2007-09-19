#ifndef __STAR
#define __STAR

#include "foliage.hpp"

class Star
{
public:
    Star();
    void update();
    void display() const;
  
private:
    Sint32 _x;
    float _y;
    float _speed;
    
    void recreate();
};

#endif
