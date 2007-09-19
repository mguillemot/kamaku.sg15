#include <iostream>
#include "graphic_types.hpp"
#include "instancizator.hpp"
#include "colors.hpp"

#ifdef __PPC__
	#include <ivga.h>
#endif

Sint32 Foliage::Instancizator::_next_free_instance = 0;
Foliage::Surface* Foliage::Instancizator::_instances[INSTANCIZATOR_MAX];

void Foliage::Instancizator::instancize(Foliage::Surface *surf)
{
	if (_next_free_instance < INSTANCIZATOR_MAX)
	{
		//TODO: YOKO mode support
		const Foliage::Size size = surf->getSize();
		Color *pixels = surf->getPixels();
		for (Sint32 i = 0; i < 16; i++)
		{
			for (Sint32 j = 15; j >= 0; j--)
			{
				if (i < size.w && (15 - j) < size.h)
				{
					ivga_write_sprite_data(_next_free_instance, 15 - j, i, *pixels);
					pixels++;
				}
				else
				{
					ivga_write_sprite_data(_next_free_instance, 15 - j, i, Foliage::Colors::Transparent);
				}
			}
		}
		_instances[_next_free_instance] = surf;
		surf->setInstancized(_next_free_instance);
		_next_free_instance++;
	}
	else
	{
		std::cout << "Impossible to instancize more surfaces." << std::endl;
	}
}
