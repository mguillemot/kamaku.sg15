#include <iostream>
#include "memory.hpp"
#include "basic_types.hpp"

Uint32 Foliage::EternalPool::current = Foliage::EternalPool::start;
Uint32 Foliage::GamePool::current = Foliage::GamePool::start;
Uint32 Foliage::LevelPool::current = Foliage::LevelPool::start;

template <class Pool>
inline void printPoolUsage()
{
	Sint32 current = (Pool::current - Pool::start + 1) >> 10;
	Sint32 total = (Pool::end - Pool::start + 1) >> 10;
	float percent = ((float)current / (float)total) * 100;
	std::cout << current << " / " << total << " KB (" << percent << "%)";
}

void Foliage::printMemoryUsage()
{
	std::cout << "Memory usage:" << std::endl;
	std::cout << " * eternal: ";
	printPoolUsage<EternalPool>();
	std::cout << std::endl;
	std::cout << " * game: ";
	printPoolUsage<GamePool>();
	std::cout << std::endl;
	std::cout << " * level: ";
	printPoolUsage<LevelPool>();
	std::cout << std::endl;
}
