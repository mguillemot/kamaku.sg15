#ifndef _FOLIAGE__SCREEN
#define _FOLIAGE__SCREEN

#include "../settings.hpp"
#include "../basic_types.hpp"
#include "graphic_types.hpp"
#include "surface.hpp"

namespace Foliage
{

	typedef void (*Synchronizator)(void);
	
	class Screen
	{
	public:
		#ifndef SCREEN_VERTICAL
		static const Sint32 Width = SCREEN_WIDTH;
		static const Sint32 Height = SCREEN_HEIGHT;
		static const ScreenMode Mode = YOKO;
		#else
		static const Sint32 Width = SCREEN_HEIGHT;
		static const Sint32 Height = SCREEN_WIDTH;
		static const ScreenMode Mode = TATE;
		#endif

		static void init();
		static void clear();
		static void fillRect(const Rect r, const Color color);
		static void setPixel(const Point pos, const Color color);
		static void flip();
		static Sint32 waitUntilEndOfFrame();
		static Synchronizator asyncBlitSection(const Uint32 sprite_addr, const Size sprite_size, 
			const Rect section, const Point pos);
		static void blitSectionSynchronizator();
		static void blitSurface(const Surface *surf, const Point pos);
		static void drawSurface(const Surface *surf, const Point pos);
		static bool instancizeSurface(const Surface *surf, const Point pos);

		// DEBUG
		static Uint32 *currentBuffer() { return (Uint32 *)_current_buffer; }

	private:
		static Uint32 _buffer[2];
		static Uint32 _current_buffer;
		static Uint32 _black_buffer;
		static volatile bool _frame_complete;
		static Sint32 _last_frame;
		static Sint32 _current_frame;
		static volatile bool _superdma_complete;
		static volatile bool _sprite_complete;
		static Uint32 _sprite_currenly_loaded;
		static Sint32 _current_instance;
		
		static void ivgaInterruptHandler(void *useless);
		static void isuperdmaInterruptHandler(void *useless);
		static void ispriteInterruptHandler(void *useless);
	
		Screen(){}
	};	
	
}

#endif //_FOLIAGE__SCREEN
