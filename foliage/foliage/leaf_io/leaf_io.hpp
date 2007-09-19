#ifndef _FOLIAGE__LEAF_IO
#define _FOLIAGE__LEAF_IO

#include <queue>
#include "../basic_types.hpp"

namespace Foliage
{

	enum Button { UP, LEFT, DOWN, RIGHT, BUTTON1, BUTTON2, BUTTON3, BUTTON4, 
		BUTTON5, BUTTON6, BUTTON7, BUTTON8, COIN, START, DEBUG };

	class InputEvent
	{
	public:
		InputEvent(const Button button, const Sint32 player, const bool pushed);
		InputEvent(const char debugChar, const Sint32 player, const bool pushed);
		Button getButton() const;
		Sint32 getPlayer() const;
		bool getPushed() const;
		char getDebugChar() const;
	
	private:
		Button _button;
		Sint32 _player;
		bool _pushed;
		char _debugChar;
	};
	
	class InputManager
	{
	public:
		static void init();
		static InputEvent nextEvent();
		static Sint32 numberOfEvents();
	
	private:
		static void updateInnerQueue();
		InputManager(){}
	
		static std::queue<InputEvent> _events;
	};

	/* INLINE IMPLEMENTATION */

	inline InputEvent::InputEvent(const Button button, const Sint32 player, const bool pushed)
		: _button(button), _player(player), _pushed(pushed), _debugChar(0)
	{
	}
	
	inline InputEvent::InputEvent(const char debugChar, const Sint32 player, const bool pushed)
		: _button(DEBUG), _player(player), _pushed(pushed), _debugChar(debugChar)
	{
	}

	inline Button InputEvent::getButton() const
	{ 
		return _button; 
	}
	
	inline Sint32 InputEvent::getPlayer() const 
	{ 
		return _player; 
	}
		
	inline bool InputEvent::getPushed() const 
	{ 
		return _pushed; 
	}
	
	inline char InputEvent::getDebugChar() const 
	{ 
		return _debugChar; 
	}
	
}

#endif //_FOLIAGE__LEAF_IO
