#ifndef _FOLIAGE__MEMORY
#define _FOLIAGE__MEMORY

#include <memory>
#include "basic_types.hpp"

namespace Foliage
{
	
	class EternalPool // (64 MB)
	{
	public:
		static const Uint32 start = 0x08000000;
		static const Uint32 end = 0x0bffffff;
		static Uint32 current;
	};
	
	class GamePool // (32 MB)
	{
	public:
		static const Uint32 start = 0x0c000000;
		static const Uint32 end = 0x0dffffff;
		static Uint32 current;
	};
	
	class LevelPool // (32 MB)
	{
	public:
		static const Uint32 start = 0x0e000000;
		static const Uint32 end = 0x0fffffff;
		static Uint32 current;
	};
	
	void printMemoryUsage();

	// Pool memory allocator. Memory can be cleared only for the whole pool at
	// one time.
	// WARNING: Pool overflow checks aren't made automatically for speed purposes.
	template <typename T, typename Pool>
	class PoolAllocator
	{
	public:

	    typedef size_t    size_type;
	    typedef ptrdiff_t difference_type;
	    typedef T         *pointer;
	    typedef const T   *const_pointer;
	    typedef T         &reference;
	    typedef const T   &const_reference;
	    typedef T         value_type;
	    
	    template <typename U>
	    struct rebind
	    {
	        typedef PoolAllocator<U, Pool> other;
	    };
	
	    PoolAllocator() throw() {}
	    
	    PoolAllocator(const PoolAllocator &) throw() {}
	    
	    template <typename U, typename P>
	    PoolAllocator(const PoolAllocator<U, P> &) throw() {}
	    
	    ~PoolAllocator() throw() {}
	    
		template <typename U, typename P>
		PoolAllocator& operator=(const PoolAllocator<U, P>&) throw()
		{
			return *this;
		}
    
	    pointer address(reference value)
	    {
	    	return &value;
	    }
	    
	    const_pointer address(const_reference value) const
	    {
	    	return &value;
	    }
	    
	    pointer allocate(size_type n, const void *hint = 0)
	    {
	    	pointer mem = (T*)Pool::current;
	    	Pool::current += n * sizeof(T);
	    	if ((Pool::current & 0x7) != 0)
	    	{
	    		// Align "Pool::current" to a multiple of 8
	    		Pool::current = (Pool::current + 8) & 0xfffffff8;
	    	}
	    	return mem;
	    }
	        
	    void deallocate(pointer addr, size_type n)
	    {
	    	// Does nothing on purpose
	    }
	    
	    size_type max_size() const throw()
	    {
	    	return ((Pool::end - Pool::start + 1) / sizeof(T));
	    }
	    
	    void construct(pointer addr, const T &value)
	    {
			new((T*)addr) T(value);
	    }
	    
	    void destroy(pointer addr)
	    {
	    	addr->~T();
	    }
	     
	    bool outOfMemory() const
	    {
	    	return (Pool::current >= Pool::end);
	    }
	    
	    void clear()
	    {
	    	Pool::current = Pool::start;
	    }
	};

	template <typename T1, typename T2, typename P1, typename P2>
	inline bool operator==(const PoolAllocator<T1, P1>&, const PoolAllocator<T2, P2>&) throw()
	{
		return (P1::start == P2::start);
	}
	
	template <typename T1, typename T2, typename P1, typename P2>
	inline bool operator!=(const PoolAllocator<T1, P1>&, const PoolAllocator<T2, P2>&) throw()
	{
		return (P1::start != P2::start);
	}

	// Partial template specialization for "void" type	
	template <typename Pool>
	class PoolAllocator<void, Pool>
	{
	public:
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
	    typedef void       *pointer;
	    typedef const void *const_pointer;
	    typedef void       value_type;
	    
	    template <typename U>
	    struct rebind
	    {
	        typedef PoolAllocator<U, Pool> other;
	    };
	};

	enum MemoryPersistance { Eternal, Game, Level };

}

void* operator new(std::size_t sz, const Foliage::MemoryPersistance pool) throw ();

void operator delete(void* m, const Foliage::MemoryPersistance pool) throw ();

void* operator new[](std::size_t sz, const Foliage::MemoryPersistance pool) throw ();

void operator delete[](void* m, const Foliage::MemoryPersistance pool) throw ();

#endif // _FOLIAGE__MEMORY
