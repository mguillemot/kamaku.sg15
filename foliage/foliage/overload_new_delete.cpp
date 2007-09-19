#include <new>
#include "memory.hpp"

Foliage::PoolAllocator<Uint8, Foliage::EternalPool> eternalAlloc;
Foliage::PoolAllocator<Uint8, Foliage::GamePool> gameAlloc;
Foliage::PoolAllocator<Uint8, Foliage::LevelPool> levelAlloc;

// For performance considerations, our pool allocators never throw std::bad_alloc
// like they should

// Normal new/delete operators

void* operator new(std::size_t sz) throw ()
{
	return (void *)eternalAlloc.allocate(sz);
}

void operator delete(void* m) throw ()
{
}

void* operator new[](std::size_t sz) throw ()
{
	return (void *)eternalAlloc.allocate(sz);
}

void operator delete[](void* m) throw ()
{
}

// Pool-specific new/delete operators

void* operator new(std::size_t sz, const Foliage::MemoryPersistance pool) throw ()
{
	switch (pool)
	{
		case Foliage::Game:
			return (void *)gameAlloc.allocate(sz);
		case Foliage::Level:
			return (void *)levelAlloc.allocate(sz);
		default:
			return (void *)eternalAlloc.allocate(sz);
	}
}

void operator delete(void* m, const Foliage::MemoryPersistance pool) throw ()
{
}

void* operator new[](std::size_t sz, const Foliage::MemoryPersistance pool) throw ()
{
	switch (pool)
	{
		case Foliage::Game:
			return (void *)gameAlloc.allocate(sz);
		case Foliage::Level:
			return (void *)levelAlloc.allocate(sz);
		default:
			return (void *)eternalAlloc.allocate(sz);
	}
}

void operator delete[](void* m, const Foliage::MemoryPersistance pool) throw ()
{
}

// No-throw new/delete operators (for STL compatibility)

void* operator new(std::size_t sz, const std::nothrow_t&) throw ()
{
	return (void *)eternalAlloc.allocate(sz);
}

void operator delete(void* m, const std::nothrow_t&) throw ()
{
}

void* operator new[](std::size_t sz, const std::nothrow_t&) throw ()
{
	return (void *)eternalAlloc.allocate(sz);
}

void operator delete[](void* m, const std::nothrow_t&) throw ()
{
}
