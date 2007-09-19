#include <iostream>
#include "leaf_io.hpp"
#include "keyboard_fr.hpp"

#ifdef __PPC__
	#include <xparameters.h>
	#include <xutil.h>
	#include <xps2.h>
	#include <xintc_l.h>
	#include <xexception_l.h>
#endif

std::queue<Foliage::InputEvent> Foliage::InputManager::_events;


#define KEYB_BUFFER_SIZE 10
#define KEYSTATE_DOWN 0
#define KEYSTATE_UP 1

typedef struct
{
    Xuint8 state;       /* state UP or DOWN of the key */
    Xuint8 key;    		/* code of the key (as VK_something) */
} virtualkey;

XPs2 Kbd; // Structure used to initialize and use the keyboard
Uint8 Kbd_Buf[1]; // Low-level keyboard buffer
Xuint16 KeyBuffer[KEYB_BUFFER_SIZE];
Xuint32 KeybRead, KeybWrite;

void intKeyboardHandler(void *KeyboardAdress, Uint32 Event, Uint32 EventData)
{
	// Static variable to use this function as a state machine
	static Uint8 state = 0;
	static Uint16 key;

	Uint8 c;

	switch (Event)
	{
		case (XPS2_EVENT_RECV_DATA):
			/* Data was received from the keyboard */
			/* We get the data from the low-level buffer */
			c = Kbd_Buf[0];
			Kbd.ReceiveBuffer.NextBytePtr--;
			Kbd.ReceiveBuffer.RemainingBytes = 1;

			/* Refer to the Wiki page "Keyboard Driver"(fr) for explanations about this automat */
			switch (state)
			{
			case 0 :
				key = 0;

				switch (c)
				{
				case 0xE0 :
					state = 5;
					break;
				case 0xF0 :
					key = 0x0100; // key released
					state++;
					break;
				default :
					key = c;
					state = 20;
					break;
				}

				break;

			case 1 :
				key |= c;
				state = 20;
				break;

			case 5:
				if (c == 0xF0)
				{
					key = 0x0100;
					break;
				}
			case 6:
				key |= c + 128;
				state = 20;
				break;

			default :
				break;
			}

			if (state == 20)
			{
				// Buffering of the character
				KeyBuffer[KeybWrite++] = key;

				KeybWrite %= KEYB_BUFFER_SIZE;

				state = 0;
			}
			break;
			
		default :
			break;
	}
}

virtualkey GetKey()
{
	// To avoid repetitions
	static Xuint16 oldc = 0;

	virtualkey vk;
	Xuint16 c;
	vk.state = 0;
	vk.key = 0;

	if (KeybRead == KeybWrite)
		return vk;

	c = KeyBuffer[KeybRead++];
	KeybRead %= KEYB_BUFFER_SIZE;

	if (c == oldc)
		return vk;

	vk.state = c >> 8;
	vk.key = c & 0xFF;

	if (!(vk.state == KEYSTATE_UP && vk.key != oldc & 0xFF))
		oldc = c;

	return vk;
}

void Foliage::InputManager::updateInnerQueue()
{
	virtualkey k = GetKey();
	while (k.state != 0 || k.key != 0)
	{
		Foliage::Button button;
		Sint32 player;
		switch (k.key)
		{
		case VK_LEFT:
			button = Foliage::LEFT;
			player = 1;
			break;
		case VK_UP:
			button = Foliage::UP;
			player = 1;
			break;
		case VK_DOWN:
			button = Foliage::DOWN;
			player = 1;
			break;
		case VK_RIGHT:
			button = Foliage::RIGHT;
			player = 1;
			break;
		case VK_L_CONTROL:
			button = Foliage::BUTTON1;
			player = 1;
			break;
		case VK_L_ALT:
			button = Foliage::BUTTON2;
			player = 1;
			break;
		case VK_SPACE:
			button = Foliage::BUTTON3;
			player = 1;
			break;
		case VK_5:
			button = Foliage::COIN;
			player = 1;
			break;
		case VK_1:
			button = Foliage::START;
			player = 1;
			break;
		default:
			k = GetKey();
			continue;
		}
		bool pushed = (k.state == KEYSTATE_DOWN);
		InputEvent e(button, player, pushed);
		_events.push(e);
		k = GetKey();
	}
}

void Foliage::InputManager::init()
{
	KeybRead = 0;
	KeybWrite = 0;

	// Low level driver init
	XPs2_mReset(XPAR_PS2_PORTS_BASEADDR_0);
	XPs2_Initialize(&Kbd, XPAR_PS2_PORTS_DEVICE_ID_0);

	// Creation of the receiving buffer
	XPs2Buffer Buff;
	Buff.NextBytePtr = Kbd_Buf;
	Buff.RemainingBytes = 1;
	Buff.RequestedBytes = 1;
	Kbd.ReceiveBuffer = Buff;

	// Interrupt init
	XPs2_SetHandler(&Kbd, (XPs2_Handler)intKeyboardHandler, (void*)&Kbd);
	XIntc_RegisterHandler(XPAR_OPB_INTC_0_BASEADDR,
		XPAR_OPB_INTC_0_PS2_PORTS_SYS_INTR1_INTR,
		(XInterruptHandler)XPs2_InterruptHandler,
		&Kbd);
	XPs2_EnableInterrupt(&Kbd);
	
	// Finished!
	std::cout << " * input manager initialized" << std::endl;
}

Foliage::InputEvent Foliage::InputManager::nextEvent()
{
	updateInnerQueue();
	Foliage::InputEvent e = _events.front();
	_events.pop();
	return e;
}

Sint32 Foliage::InputManager::numberOfEvents()
{
	updateInnerQueue();
	return _events.size();
}
