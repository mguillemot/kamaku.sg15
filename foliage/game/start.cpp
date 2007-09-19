#include <string>
#include <iostream>
#include <list>
#include "foliage.hpp"
#include "bullet.hpp"
#include "star.hpp"
#include "font.hpp"

using namespace std;
using namespace Foliage;

#define NB_STARS 0

void append_string(string &s, Sint32 n, Sint32 min_len = 1)
{
	char buf[10]; // max=2147483647
	int buf_len = 0;
	if (n == 0)
	{
		s += '0';
		return;
	}
	else if (n < 0)
	{
		s += '-';
		n = -n;
	}
	while (n != 0)
	{
		int digit = n % 10;
		n /= 10;
		buf[buf_len++] = '0' + digit;
	}
	for (Sint32 i = 0; i < (min_len - buf_len - 1); ++i)
	{
		s += '0';
	}
	while (buf_len > 0)
	{
		s += buf[--buf_len];
	}
}

Bullet *fire_at(const Foliage::Point from, const Foliage::Point to, const Fixed speed, const Fixed shift_angle)
{
    Fixed angle;
    if (to.x < 0 && to.y < 0)
    {
        angle = shift_angle;
    }
    else
    {
        Sint32 dx = to.x - from.x;
        Sint32 dy = to.y - from.y;
        if (dx != 0)
        {
            angle = Foliage::FastMath::atan(Fixed(Sint32(dy)) / Fixed(int(dx)));
        }
        else if (dy > 0)
        {
            angle = F_PI_2;
        }
        else
        {
            angle = F_MINUS_PI_2;
        }
        if (dx < 0)
        {
            angle += F_PI;
        }
        angle += shift_angle;
    }
    Bullet *shot = new Bullet(from, angle, speed);
    return shot;
}

void start()
{
	/*
	Foliage::Sound bg("kloops.wav");
	cout << "bg music has " << bg.getSamplesNb() << " samples" << endl;
	Foliage::Sound hit("break.wav");
	cout << "SFX has " << hit.getSamplesNb() << " samples" << endl;
	*/
	SoundManager::disableSound();

	Sint32 i;
	Sint32 skipped = 0;
	typedef list<Bullet*, PoolAllocator<Bullet*, LevelPool> > ListBullet;
	typedef list<Star*> ListStar;
	ListBullet bullets;
	ListStar stars;
    for (i = 0; i < NB_STARS; i++)
    {
        stars.push_back(new Star());
    }
    Font font;
    cout << "Font loaded." << endl;
    Foliage::Sprite background("bg.bmp"); // 300x3910
    Foliage::Sprite background1(background.getCurrentSurface()->createNewShiftedSurface(1));
    Foliage::Sprite background2(background.getCurrentSurface()->createNewShiftedSurface(2));
    Foliage::Sprite background3(background.getCurrentSurface()->createNewShiftedSurface(3));
	Foliage::Sprite ship("vaiss00.bmp");
	ship.addFrame("vaiss01.bmp");
	ship.addFrame("vaiss02.bmp");
	ship.addFrame("vaiss03.bmp");
	ship.addFrame("vaiss04.bmp");
	ship.addFrame("vaiss05.bmp");
    Rect shipHitbox = Rect(10, 11, 8, 9);
    ship.setHitbox(shipHitbox);
    ship.setPosition(Point(100, 250));
    Foliage::Sprite enemy("ennemi.bmp");
    Rect enemyHitbox = Rect(4, 13, 30, 15);
    enemy.setHitbox(enemyHitbox);
    enemy.setPosition(Point(100, 20));
	Speed s;
	Sint32 frame = 0;
	const Fixed ShipSpeed = Fixed(1.5f);
	Sint32 hitCount = 0;
	Sint32 randlevel = 10;

	//SoundManager::playBg(&bg);		
	
	while (true)
	{
		while (Foliage::InputManager::numberOfEvents() > 0)
		{
			InputEvent e = Foliage::InputManager::nextEvent();
			Fixed adj_speed = (e.getPushed()) ? ShipSpeed : ShipSpeed.opposite();
			if (e.getPlayer() == 1)
			{
				if (e.getButton() == LEFT)
				{
					s.x -= adj_speed;
				}
				else if (e.getButton() == RIGHT)
				{
					s.x += adj_speed;
				}
				else if (e.getButton() == UP)
				{
					s.y -= adj_speed;
				}
				else if (e.getButton() == DOWN)
				{
					s.y += adj_speed;
				}
				else if (e.getButton() == BUTTON1 && e.getPushed())
				{
					randlevel++;
				}
				else if (e.getButton() == BUTTON2 && e.getPushed())
				{
					if (randlevel >= 2)
					{
						randlevel--;
					}
				}
			}
		}
		ship.setSpeed(s);
		Rect section;
		section.w = 240;
		section.h = 320;
		section.x = ship.getPosition().x / 4;
		section.y = background.getSize().h - 320 - frame;
		if (section.y < 0)
			section.y = 0;
		Synchronizator waitEndOfBg;
		if (section.y % 4 == 0)
		{
			waitEndOfBg = Screen::asyncBlitSection((Uint32)background.getCurrentSurface()->getPixels(), background.getSize(), section, Point(0, 0));
		}
		else if (section.y % 4 == 1)
		{
			waitEndOfBg = Screen::asyncBlitSection((Uint32)background3.getCurrentSurface()->getPixels(), background3.getSize(), section, Point(0, 0));
		}
		else if (section.y % 4 == 2)
		{
			waitEndOfBg = Screen::asyncBlitSection((Uint32)background2.getCurrentSurface()->getPixels(), background2.getSize(), section, Point(0, 0));
		}
		else
		{
			waitEndOfBg = Screen::asyncBlitSection((Uint32)background1.getCurrentSurface()->getPixels(), background1.getSize(), section, Point(0, 0));
		}
		ship.move();
		enemy.move();
		/* wrong place due to bg sync
        for (ListStar::iterator i = stars.begin(); i != stars.end(); i++)
        {
            (*i)->display();
            (*i)->update();
        }
        */
		if ((rand() % randlevel) == 0)
		{
			for (Sint32 j = -30; j <= 30; j++)
			{
				Bullet *shot = fire_at(enemy.getCenter(), ship.getCenter(), Fixed(4), Fixed(j) / 15);
				bullets.push_back(shot);
			}
		}
		waitEndOfBg();
		string frame_nb = "frame #";
		append_string(frame_nb, frame);
		string bullets_nb;
		append_string(bullets_nb, bullets.size(), 4);
		bullets_nb += " bullets";
		string frameskip_nb = "frameskip ";
		append_string(frameskip_nb, skipped);
		string randlevel_nb = "level ";
		append_string(randlevel_nb, randlevel);
		ship.draw();
		enemy.draw();
		const Rect shipHb = ship.getScreenHitbox();
		ListBullet::iterator i = bullets.begin();
		while (i != bullets.end())
		{
			Bullet *b = *i;
			b->update();
			if (b->getSprite()->outOfScreen())
			{
				i = bullets.erase(i);
				delete b;
			}
			else
			{
				b->getSprite()->draw();
				Rect bHb = b->getSprite()->getScreenHitbox();
				if (Rect::intersects(shipHb, bHb))
				{
					i = bullets.erase(i);
					delete b;
					hitCount++;
				}
				else
				{
					i++;
				}
			}
		}
		font.drawString(frame_nb, Point(0, 0));
		font.drawString(bullets_nb, Point(0, 17));
		font.drawString(frameskip_nb, Point(0, 34));
		font.drawString(randlevel_nb, Point(0, 51));
		if ((frame % 1000) == 0)
		{
			cout << bullets.size() << " bullets!" << endl;
			printMemoryUsage();
		}
		if (frame % 300 == 0)
		{
			//SoundManager::playSfx(&hit);
		}
		skipped = Screen::waitUntilEndOfFrame();
		if (skipped > 0)
		{
			Screen::setPixel(Point(0, 0), Colors::Green);
		}
		else
		{
			Screen::setPixel(Point(0, 0), Colors::Black);
		}
		Screen::flip();
		frame++;
	}
}

int main()
{
	/*
	XCache_DisableDCache();
	
	Sint32 *bidon_int = (Sint32 *)0x08000000;
	float *bidon_float = (float *)0x08000004;
	Sint32 c;
	float aa, bb, cc;
	Sint32 aaa, bbb, ccc;
	Timer t1;
	t1.start();
	for (c = 0; c < 1000; c++)
	{
		aa = atan(c);
		*bidon_float = aa;
	}
	t1.stop();
	Timer t2;
	t2.start();
	for (c = 0; c < 1000; c++)
	{
		aa = fast_atan(c);
		*bidon_float = aa;
	}
	t2.stop();
	cout << "v1=" << (Sint32)(t1.duration() * 1000000) << "us / v2=" 
		<< (Sint32)(t2.duration() * 1000000) << "us" << endl;
	while(1);
	*/	

	Foliage::init();
	start();
	while (true)
	{
	}
	return 0;
}
